#include "motor.h"
#include <avr/io.h>
#include <Arduino.h>

// put defines to motor.h?
#define MOTOR_PORT PORTE
#define MOTOR_DDR DDRE
#define MOTOR_PIN_L PE7
#define MOTOR_PIN_R PE6

#define MOTOR_SENSE_DDR DDRE
#define MOTOR_SENSE_PORT PORTE
#define MOTOR_SENSE_PORT_IN PINE	// ??

#define MOTOR_SENSE_LED_PIN	PE2
#define MOTOR_SENSE_PIN PE3

#define ADC_CH_MOTOR 2
#define MOTOR_MIN_CURRENT 900

#define MOTOR_MAX_CLOSE 850		// max number of reflex counts, prevents loose of piston

// delay counter for motor stop move detection
#define MOTOR_TIMER_DELAY 3				// normal delay
#define MOTOR_TIMER_START_DELAY 10		// longer delay for startup


void motor::init()
{
	// set motor control pins to off
	MOTOR_PORT &= ~((1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R));

  	// set motor sense led pin to output
	MOTOR_SENSE_DDR |= (1 << MOTOR_SENSE_LED_PIN);

	// set motor sense pin to input
	MOTOR_SENSE_DDR &= ~(1 << MOTOR_SENSE_PIN);	// Input

	// enable interrupt of motor sense
	//EICRA |= ((1 << ISC00) | (1 << ISC01));	// falling edge?
	EIMSK |= (1 << PCIE0);	// PC-INT 0..7
	PCMSK0 |= (1 << MOTOR_SENSE_PIN);			// PCINT3
}


void motor::open()
{
	if (m_direction == FULLOPEN)
		return;

	m_direction = OPEN;
	timerDelay = MOTOR_TIMER_START_DELAY;

	// disable close first!!
	MOTOR_DDR &= ~(1 << MOTOR_PIN_R);
	MOTOR_PORT &= ~(1 << MOTOR_PIN_R);

	// switch on sense LED
	MOTOR_PORT |= (1 << MOTOR_SENSE_LED_PIN);

	// enable open
	MOTOR_DDR |= (1 << MOTOR_PIN_L);
	MOTOR_PORT |= (1 << MOTOR_PIN_L);

	running = true;
}


void motor::close()
{
	if (m_direction == FULLCLOSE)
		return;

	m_direction = CLOSE;
	timerDelay = MOTOR_TIMER_START_DELAY;

	// disable open first!!
	MOTOR_DDR &= ~(1 << MOTOR_PIN_L);
	MOTOR_PORT &= ~(1 << MOTOR_PIN_L);

	// switch on sense LED
	MOTOR_PORT |= (1 << MOTOR_SENSE_LED_PIN);

	// enable close
	MOTOR_DDR |= (1 << MOTOR_PIN_R);
	MOTOR_PORT |= (1 << MOTOR_PIN_R);

	running = true;
}


void motor::stop()
{
	if (!running)
		return;

	// disable motor control pins
	MOTOR_DDR &= ~((1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R));
	MOTOR_PORT &= ~((1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R));

	// switch off sense LED
	MOTOR_PORT &= ~(1 << MOTOR_SENSE_LED_PIN);

	running = false;
	timerDelay = MOTOR_TIMER_DELAY;

	if (position < 0) position = 0;
}


// pos: open = 0, close = 255
void motor::moveTo(uint8_t pos)
{
	target = pos * (valveClosedPosition / 255);

	if (pos == 255)
		target = 10000;		// FULLCLOSE; MOTOR_MAX_CLOSE;

	if (pos == 0)
		target = -10000;	// FULLOPEN

	if (position < target)
	{
		close();
	}

	if (position > target)
	{
		open();
	}
}


uint16_t motor::getCurrent()
{
	return analogRead(ADC_CH_MOTOR);
}


// must be called by PCINT0_vect
void motor::motorISR()
{
	if (!running)
		return;

	if (m_direction == OPEN)
	{
		position--;
		if (position <= target)
			stop();
	}

	if (m_direction == CLOSE)
	{
		position++;
		if (position >= MOTOR_MAX_CLOSE || position >= target)
			stop();
	}
}


void motor::endPositionReached()
{
	stop();

	if (m_direction == OPEN)
	{
		m_direction = FULLOPEN;	
		position = 0;
	}

	if (m_direction == CLOSE)
	{
		m_direction = FULLCLOSE;
		valveClosedPosition = position;
	}
}


// periodically called (LCD interrupt?)
void motor::timer()
{
	static int lastPosition = 0;	// init to defaults

	if (running)
	{
		// stop motor if position have not changed for a long period
		if (--timerDelay == 0)
		{
			timerDelay = MOTOR_TIMER_DELAY;

			if (lastPosition == position)
			{
				endPositionReached();
				return;
			}
			lastPosition = position;
		}

		// stop motor if current is to high
		if (getCurrent() < MOTOR_MIN_CURRENT)
		{
			endPositionReached();
		}
	}
}

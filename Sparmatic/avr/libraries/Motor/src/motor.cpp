#include "motor.h"
#include <avr/io.h>
#include <Arduino.h>

#define MOTOR_PORT PORTE
#define MOTOR_DDR DDRE
#define MOTOR_PIN_L PE7
#define MOTOR_PIN_R PE6

#define MOTOR_SENSE_DDR DDRE
#define MOTOR_SENSE_PORT PORTE
#define MOTOR_SENSE_PORT_IN PINE
#define MOTOR_SENSE_LED_PIN PE2

motor::motor() : m_direction(STOP)
{
	
}

void motor::init(){
	// set motor control pins to off
	MOTOR_PORT &= ~((1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R));
	// set motor sense led pin to output
	MOTOR_SENSE_DDR |= (1 << MOTOR_SENSE_LED_PIN);
	// enable interrupt of motor sense
	PCMSK0 |= (1 << PCINT1);
}

inline void motor::open(){
	// disable close
	MOTOR_DDR &= ~(1 << MOTOR_PIN_R);
	MOTOR_PORT &= ~(1 << MOTOR_PIN_R);

	// enable open
	MOTOR_DDR |= (1 << MOTOR_PIN_L);
	MOTOR_PORT |= (1 << MOTOR_PIN_L);
}

inline void motor::close(){
	// disable open
	MOTOR_DDR &= ~(1 << MOTOR_PIN_L);
	MOTOR_PORT &= ~(1 << MOTOR_PIN_L);

	// enable close
	MOTOR_DDR |= (1 << MOTOR_PIN_R);
	MOTOR_PORT |= (1 << MOTOR_PIN_R);
}

inline void motor::stop(){
	// disable motor control pins
	MOTOR_DDR &= ~(1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R);
	MOTOR_PORT &= ~((1 << MOTOR_PIN_L) | (1 << MOTOR_PIN_R));
}

void motor::move(enum MotorDir direction)
{
	m_direction = direction;
	stop();

	switch(direction)
	{
		case CLOSE:
			close();
			break;
		case OPEN:
			open();
		case STOP:
			// other stuff need to be done
			break;
	}
}
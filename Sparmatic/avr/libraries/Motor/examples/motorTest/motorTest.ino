#include <motor.h>
#include <input.h>
#include <lcd.h>

void setup()
{
	LCD.init();
	Input.init();
	Motor.init();	
}

void loop()
{
    int8_t value = Input.encoderRead();
	LCD.displayNumber(value, 4);
	Motor.move((enum MotorDir)value);
	delay(1000);
}

ISR(LCD_vect)
{
    Input.periodicScan();
    
}

ISR(PCINT1_vect)
{
	Input.periodicScan();
}


#define PCINT0_PORTIN PINE
ISR(PCINT0_vect)
{
	static unsigned char lastState = 0; // init to defaults
	unsigned char newState = PCINT0_PORTIN;
	unsigned char changed = newState ^ lastState;
	lastState = newState;
	
	// motor step
	if(changed & (1 << MOTOR_SENSE_PIN))
	{
		Motor.step();
	}
}
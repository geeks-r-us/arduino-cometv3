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
    int8_t enc = Input.get_key_increment();

	if (enc == 1)
	{ // Open
		Motor.moveTo(0);
	}

	if (enc == -1)
	{ // Close
		Motor.moveTo(255);
	}

	if (Motor.running)
		LCD.displayNumber(Motor.position, 4);

/*
	LCD.displayNumber(value, 4);
	Motor.move((enum MotorDir)value);
	delay(1000);
*/
}

ISR(LCD_vect)
{
    Input.periodicScan();

 	Motor.timer();   
}

ISR(PCINT1_vect)
{
	Input.periodicScan();
}


#define PCINT0_PORTIN PINE
ISR(PCINT0_vect)
{
	Motor.motorISR();
}

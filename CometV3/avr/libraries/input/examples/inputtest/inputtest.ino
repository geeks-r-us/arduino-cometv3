#include <lcd.h>
#include <input.h>
#include <stdint.h>


int32_t value = 0;

void setup()
{
    LCD.init();
    Input.init();
}

void loop()
{

    uint8_t keys = Input.get_key_press((1 << KEY_OK) | (1 << KEY_MENU) | (1 << KEY_CLOCK) );

    
    LCD.displaySymbols(keys, keys);
	

    value += Input.encoderRead();
    LCD.displayNumber(value, 4);
}

ISR(LCD_vect)
{
    Input.periodicScan();
    
}

ISR(PCINT1_vect)
{
	LCDCRA |= (1 << LCDIE);
	
	Input.periodicScan();
}
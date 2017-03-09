/**
 * Frontend demo for Thermy V3
 * simple ragulation implemented
 * regulation is running in the "REGL" menu
 */

#include <Arduino.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include <lcd.h>
#include <input.h>
#include <stdint.h>
#include <motor.h>
#include <ntc.h>

#include <Menu.h>


// helperfunction
void menu_return_to_main()
{
	Menu.handleMenu = MainMenuHandler;
}

//----------------------------------------------------------------------
const MENU_ENTRY ModeMenuEntries[] PROGMEM = {
	{"AUTO", NULL},
	{"MANU", NULL}
};
MENU ModeMenu = { 0, 1, ModeMenuEntries};

void ModeMenu_function()
{
	Menu.displayEntry(&ModeMenu);

	int8_t enc = Input.get_key_increment();
	if (enc == 1) { Menu.nextEntry(&ModeMenu);}
	if (enc == -1) { Menu.prevEntry(&ModeMenu);}

	if ( Input.get_key_press(1 << KEY_OK) )
		menu_return_to_main();
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
int8_t window_time = 10;
void WindowTime_function()
{
	int8_t enc = Input.get_key_increment();
	window_time += enc * 10;
	if (window_time < 10) window_time = 60;
	if (window_time > 60) window_time = 10;
	LCD.displayNumber(window_time, 2);

	if ( Input.get_key_press(1 << KEY_OK) )
		menu_return_to_main();
}

const MENU_ENTRY WindowMenuEntries[] PROGMEM = {
	{"HOCH", WindowTime_function},
	{"MITT", WindowTime_function},
	{"NIED", WindowTime_function}
};
MENU WindowMenu = { 0, 2, WindowMenuEntries };

void WindowMenu_function()
{
	Menu.displayEntry(&WindowMenu);

	int8_t enc = Input.get_key_increment();
	if (enc == 1) { Menu.nextEntry(&WindowMenu);}
	if (enc == -1) { Menu.prevEntry(&WindowMenu);}

	if ( Input.get_key_press(1 << KEY_OK) )
	{
		LCD.displayString("...M");
		Menu.handleMenu = WindowTime_function;
	}
	if ( Input.get_key_press(1 << KEY_MENU) )
		menu_return_to_main();
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
int8_t offset_temp = 0;
void OffsetMenu_function()
{
	LCDDR7 |= (1 << 7);			// DOT on

	int8_t enc = Input.get_key_increment();
	offset_temp += enc * 5;
	if (offset_temp < -50) offset_temp = -50;
	if (offset_temp > 50) offset_temp = 50;

	LCD.displayNumber(offset_temp, 3);
	LCD.displayAsciiDigit('@', 3);

	if ( Input.get_key_press(1 << KEY_OK) )
	{
		LCDDR7 &= ~(1 << 7);	// DOT off
		menu_return_to_main();
	}
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
void MotorTest_function()
{
	LCD.displayString("MOTO");

	int8_t enc = Input.get_key_increment();

	if (enc == 1)
	{ // Open
		Motor.moveTo(0);
	}

	if (enc == -1)
	{ // Close
		Motor.moveTo(255);
	}

	if ( Input.get_key_press(1 << KEY_OK) )
	{
//		Motor.move(STOP);
	}

	if ( Input.get_key_press(1 << KEY_MENU) )
	{
//		Motor.move(STOP);
		menu_return_to_main();
	}

	if (Motor.running)
		LCD.displayNumber(Motor.position, 4);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
void Adap()
{
	LCD.displayString(">>>>");
	if (!Motor.running)
	{
		menu_return_to_main();
	}
}

void InstallMenu_function()
{
	if (!Motor.running && !(Motor.m_direction == FULLOPEN))
	{
		Motor.moveTo(0);
		LCD.displayString("<<<<");
	}

	if (Motor.m_direction == FULLOPEN)
	{
		LCD.displayString("INST");
		if ( Input.get_key_press(1 << KEY_OK) )
		{
			Motor.moveTo(255);
			Menu.handleMenu = Adap;
		}
	}

	if ( Input.get_key_press(1 << KEY_MENU) )
	{
		menu_return_to_main();
	}
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
bool regOn = false;

void ShowTemps_function()
{
	regOn = true;

	LCDDR7 |= (1 << 7);			// DOT on

	int16_t temp = NTC.getTemperature();
	LCD.displayNumber(temp, 4);

	if ( Input.get_key_press(1 << KEY_MENU) )
	{
		LCDDR7 &= ~(1 << 7);	// DOT off
		regOn = false;

		menu_return_to_main();
	}
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
uint16_t targetTemp = 210;

void SetTemp_function()
{
	LCDDR7 |= (1 << 7);			// DOT on

	int8_t enc = Input.get_key_increment();
	targetTemp += enc * 5;
	if (targetTemp < 50) targetTemp = 50;		// 5 Deg
	if (targetTemp > 500) targetTemp = 500;		// 50 Deg

	LCD.displayNumber(targetTemp, 3);
	LCD.displayAsciiDigit('@', 3);

	if ( Input.get_key_press(1 << KEY_OK) )
	{
		LCDDR7 &= ~(1 << 7);	// DOT off
		menu_return_to_main();
	}
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
const MENU_ENTRY MainMenuEntries[] PROGMEM = {
	{"MODE", ModeMenu_function},
	{"TEMP", SetTemp_function},
	{"REGL", ShowTemps_function},
	{"FENS", WindowMenu_function},
	{"RES.", NULL},
	{"ADAP", InstallMenu_function},
	{"URLA", NULL},
	{"VENT", NULL},
	{"OFFS", OffsetMenu_function},
	{"DBUG", MotorTest_function}
};
MENU MainMenu = { 0, 9, MainMenuEntries };

void MainMenuHandler()
{
	Menu.displayEntry(&MainMenu);

	int8_t enc = Input.get_key_increment();

	if (enc == 1) { Menu.nextEntry(&MainMenu);}
	if (enc == -1) { Menu.prevEntry(&MainMenu);}

	if ( Input.get_key_press(1 << KEY_OK) ) { Menu.execEntry(&MainMenu); }
}
//----------------------------------------------------------------------


void setup()
{
	LCD.init();
	Input.init();
	Motor.init();
	NTC.init();

	Menu.handleMenu = InstallMenu_function;
//	Menu.handleMenu = MainMenuHandler;
}

// very simple ragulation
void regulate()
{
	if (!regOn)
		return;

	static int16_t deltaBefore = 0;
	static int16_t position = 255;		// init position, valve closed

	NTC.updateTemperature();
	int16_t delta = NTC.getTemperature() - (targetTemp * 10);

	int way = 0;
	if (delta > 0 && delta > deltaBefore)
	{ // Temp high and rising
		if (delta >= 10) way = 3;		// 1% // 10
		if (delta >= 20) way = 6;		// 2% // 20
		if (delta >= 100) way = 128;	// 50% // 100
		if (delta >= 250) way = 200;	// 80% // 300
	}

	if (delta < 0 && delta < deltaBefore)
	{ // Temp low and falling
		if (delta <= -10) way = -3;		// -1% // -10
		if (delta <= -20) way = -6;		// -2% // -20
		if (delta <= -100) way = -128;	// -50% // -100
		if (delta <= -250) way = -200;	// -80% // -300
	}

	position += way;
	if (position < 0) position = 0;
	if (position > 255) position = 255;

	Motor.moveTo((uint8_t)position);

	deltaBefore = delta;
}


void loop()
{
	delay(30000);
	regulate();
}


ISR(LCD_vect)
{
	Input.periodicScan();
	Menu.handleMenu();
	Motor.timer();
}

ISR(PCINT1_vect)
{
	LCDCRA |= (1 << LCDIE);		// ??
	Input.periodicScan();
}

ISR(PCINT0_vect)
{
	Motor.motorISR();
}


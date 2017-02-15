/**
 * Frontend demo for Thermy V3
 */

#include <Arduino.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include <lcd.h>
#include <input.h>
#include <stdint.h>
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
  LCDDR7 |= (1 << 7);   // DOT on

  int8_t enc = Input.get_key_increment();
  offset_temp += enc * 5;
  if (offset_temp < -50) offset_temp = -50;
  if (offset_temp > 50) offset_temp = 50;

  LCD.displayNumber(offset_temp, 3);
  LCD.displayAsciiDigit('@', 3);

  if ( Input.get_key_press(1 << KEY_OK) )
  {
    LCDDR7 &= ~(1 << 7);   // DOT off
    menu_return_to_main();
  }
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
uint16_t targetTemp = 210;

void SetTemp_function()
{
  LCDDR7 |= (1 << 7);   // DOT

  int8_t enc = Input.get_key_increment();
  targetTemp += enc * 5;
  if (targetTemp < 50) targetTemp = 50;      // 5 Deg
  if (targetTemp > 500) targetTemp = 500;    // 50 Deg

  LCD.displayNumber(targetTemp, 3);
  LCD.displayAsciiDigit('@', 3);

  if ( Input.get_key_press(1 << KEY_OK) )
  {
    LCDDR7 &= ~(1 << 7);   // DOT
    menu_return_to_main();
  }
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// will be called from DBUG menu
void ShowTemps_function()
{
  LCDDR7 |= (1 << 7);   // DOT on

  NTC.updateTemperature();
  int16_t temp = NTC.getTemperature();
  LCD.displayNumber(temp/10, 3);
  LCD.displayAsciiDigit('@', 3);

  if ( Input.get_key_press(1 << KEY_MENU) )
  {
    LCDDR7 &= ~(1 << 7);   // DOT off
    menu_return_to_main();
  }
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
const MENU_ENTRY MainMenuEntries[] PROGMEM = {
  {"MODE", ModeMenu_function},
  {"PROG", NULL},
  {"TEMP", SetTemp_function},
  {"FENS", WindowMenu_function},
  {"RES.", NULL},
  {"ADAP", NULL},
  {"URLA", NULL},
  {"VENT", NULL},
  {"OFFS", OffsetMenu_function},
  {"DBUG", ShowTemps_function}
};
MENU MainMenu = { 0, 9, MainMenuEntries };

void MainMenuHandler()
{
  Menu.displayEntry(&MainMenu);

  int8_t enc = Input.get_key_increment();

  if (enc == 1) { Menu.nextEntry(&MainMenu);}
  if (enc == -1) { Menu.prevEntry(&MainMenu);}

  if ( Input.get_key_press(1 << KEY_OK) )
    Menu.execEntry(&MainMenu);
}
//----------------------------------------------------------------------


void setup()
{
  LCD.init();
  Input.init();
  NTC.init();

  Menu.handleMenu = MainMenuHandler;
}

void loop()
{

}


ISR(LCD_vect)
{
  Input.periodicScan();
  Menu.handleMenu();
}

ISR(PCINT1_vect)
{
  LCDCRA |= (1 << LCDIE);   // ??

  Input.periodicScan();
}


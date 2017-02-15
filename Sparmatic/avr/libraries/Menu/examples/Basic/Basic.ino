/**
 * Basic Menu
 */

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <lcd.h>
#include <input.h>
#include <stdint.h>

#include <Menu.h>

//----------------------------------------------------------------------
// this is a sub-menu with 2 entries, will be called from the MainMenu

const MENU_ENTRY ModeMenuEntries[] PROGMEM = {
  {"AUTO", NULL},
  {"MANU", NULL}
};
MENU ModeMenu = { 0, 1, ModeMenuEntries};

// every menu should have a function to handle its functionality 
void ModeMenu_function()
{
  Menu.displayEntry(&ModeMenu);

  int8_t enc = Input.get_key_increment();
  if (enc == 1) { Menu.nextEntry(&ModeMenu);}
  if (enc == -1) { Menu.prevEntry(&ModeMenu);}

  if ( Input.get_key_press(1 << KEY_OK) )
    Menu.handleMenu = MainMenuHandler;    // back to MainMenu
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// set temperature, called from MainMenu
uint16_t temperature = 210;

// every menu should have a function to handle its functionality 
void TempMenu_function()
{
  LCDDR7 |= (1 << 7);   // DOT on

  int8_t enc = Input.get_key_increment();
  temperature += enc * 5;
  if (temperature < 50) temperature = 50;      // 5 Deg
  if (temperature > 500) temperature = 500;    // 50 Deg

  LCD.displayNumber(temperature, 3);
  LCD.displayAsciiDigit('@', 3);      // Degree sign

  if ( Input.get_key_press(1 << KEY_OK) )
  {
    LCDDR7 &= ~(1 << 7);   // DOT off
    Menu.handleMenu = MainMenuHandler;    // back to MainMenu
  }
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
const MENU_ENTRY MainMenuEntries[] PROGMEM = {
  {"MODE", ModeMenu_function},
  {"PROG", NULL},
  {"TEMP", TempMenu_function}
};
MENU MainMenu = { 0, 2, MainMenuEntries };

void MainMenuHandler()
{
  Menu.displayEntry(&MainMenu);

  int8_t enc = Input.get_key_increment();

  if (enc == 1) { Menu.nextEntry(&MainMenu);}
  if (enc == -1) { Menu.prevEntry(&MainMenu);}

  // call the function of the selected menu
  if ( Input.get_key_press(1 << KEY_OK) )
    Menu.execEntry(&MainMenu);
}
//----------------------------------------------------------------------


void setup()
{
  LCD.init();
  Input.init();

  // we initialize the Menu callback with the MainMenu
  Menu.handleMenu = MainMenuHandler;
}

void loop()
{
// we have notthing to do here, the menu will be handled in interrupt
}


ISR(LCD_vect)
{
  Input.periodicScan();

  // call peridically
  Menu.handleMenu();
}

ISR(PCINT1_vect)
{
  LCDCRA |= (1 << LCDIE);   // ??

  Input.periodicScan();
}


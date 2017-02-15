/*
 * Menu.cpp
 *
 */

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <lcd.h>
#include "Menu.h"

void Menu::nextEntry(MENU *Menu)
{
	Menu->pos++;

	if (Menu->pos < 0) Menu->pos = 0;
	if (Menu->pos > Menu->max) Menu->pos = Menu->max;
}

void Menu::prevEntry(MENU *Menu)
{
	Menu->pos--;

	if (Menu->pos < 0) Menu->pos = 0;
	if (Menu->pos > Menu->max) Menu->pos = Menu->max;
}

// execute fp (go to sub menu, see example)
void Menu::execEntry(MENU *Menu)
{
	void (*fp) = pgm_read_word( &Menu->menu[Menu->pos].fp );

	if (fp == NULL)
		return;

	handleMenu = fp;
	handleMenu();   // not necessary? (called by interrupt)
}

// show text on LCD
void Menu::displayEntry(MENU *Menu)
{
	char text[4];

	for (uint8_t i = 0; i < sizeof(text); i++)
	{
		text[i] = pgm_read_byte( &Menu->menu[Menu->pos].text[i] );
	}

	LCD.displayString(text);
}

#ifndef MENU_H_
#define MENU_H_

typedef struct
{
	char text[4];		// text to be shown on LCD
	void (*fp)();		// function pointer or NULL
} MENU_ENTRY;

typedef struct
{
	int pos;			// current menu entry
	int max;			// number of entries -1
MENU_ENTRY *menu;		// array of MENU_ENTRY
} MENU;

class Menu {
public:
	void (*handleMenu)();			// callbak for menu (interrupt?)
	void nextEntry(MENU *Menu);
	void prevEntry(MENU *Menu);
	void execEntry(MENU *Menu);		// execute fp (go to sub menu)
	void displayEntry(MENU *Menu);	// show text on LCD
};

static Menu Menu;

#endif /* MENU_H_ */

#ifndef LCD_H_
#define LCD_H_

typedef enum {
	LCD_AUTO		 =    1,
	LCD_MANU		 =    2,
	LCD_BAG			 =    4,
	LCD_TOWER		 =    8,
	LCD_INHOUSE	     =   16,
	LCD_OUTHOUSE     =   32,
	LCD_MOON		 =   64,
	LCD_STAR		 =  128,
	LCD_BATTERY	     =  256,
	LCD_LOCK		 =  512,
	LCD_DP			 = 1024,	// colon?
	LCD_HOURS		 = 2048
} LCD_SYMBOLS;

#define LCD_SYM_ALL (2 * LCD_HOURS - 1)

class LCD {
public:
    void init(void);
    void displayAsciiDigit(char c, uint8_t pos);
    void displayString(char *str);
    void displayNumber(int16_t num, int8_t width);
    void displayBargraph(uint32_t bargraphOn);
    void displayWeekday(uint8_t dayOn);
    void displaySymbols(LCD_SYMBOLS on, LCD_SYMBOLS mask);
    void off(void);

private:
    void displayDigit(uint16_t segments, uint8_t pos);
    void segmentSwitch(uint8_t segment, uint8_t on);
    void segmentOff(uint8_t segment);
    void segmentOn(uint8_t segment);

};

static LCD LCD;


#endif /* LCD_H_ */

#include <lcd.h>
#include <ntc.h>

void setup()
{
    LCD.init();
    NTC.init();
}

void loop()
{
    NTC.updateTemperature();
    int16_t temp = NTC.getTemperature();
    temp = NTC.getNtcAdc();
    LCD.displayNumber(temp, 4);
}
#include <lcd.h>
#include <input.h>
#include <ntc.h>
#include <motor.h>

int32_t value = 0;

void setup()
{
  LCD.init();
  Input.init();
  NTC.init();
  Motor.init();
}

void loop()
{
  uint8_t keys = Input.get_key_press((1 << KEY_OK) | (1 << KEY_MENU) | (1 << KEY_CLOCK));

  LCD.displaySymbols(keys, keys);

  value += Input.encoderRead();
  LCD.displayNumber(value, 4);
}

#define PCINT0_PORTIN PINE
ISR(PCINT0_vect)
{
  static unsigned char lastState = 0; // init to defaults
  unsigned char newState = PCINT0_PORTIN;
  unsigned char changed = newState ^ lastState;
  lastState = newState;

  // save data when battery removed
  //if (newState & (1 << POWERLOSS_PIN))
  //{
    //sysShutdown();
  //}

  // motor step
  if (changed & (1 << MOTOR_SENSE_PIN))
  {
    Motor.step();
  }
}


ISR(LCD_vect)
{
  Input.periodicScan();
}

ISR(PCINT1_vect)
{
  LCDCRA |= (1 << LCDIE);

  Motor.timer();

  Input.periodicScan();
}

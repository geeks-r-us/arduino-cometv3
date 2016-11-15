/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            48
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 54 : -1)
#define digitalPinHasPWM(p)         ((p) >= 14 && (p) <= 17) 

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (12)
#define PIN_SPI_MISO  (13)
#define PIN_SPI_SCK   (11)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (7)
#define PIN_WIRE_SCL        (6)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13

#define PIN_A0   (54)
#define PIN_A1   (55)
#define PIN_A2   (56)
#define PIN_A3   (57)
#define PIN_A4   (58)
#define PIN_A5   (59)
#define PIN_A6   (60)
#define PIN_A7   (61)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;


// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):
// (I've deliberately left out pin mapping to the Hardware USARTs - seems senseless to me)
// Pins: 10, 11, 12, 13,  50, 51, 52, 53,  62, 63, 64, 65, 66, 67, 68, 69

#define digitalPinToPCICR(p)    ( (((p) >= 10) && ((p) <= 13)) || \
                                  (((p) >= 50) && ((p) <= 53)) || \
                                  (((p) >= 62) && ((p) <= 69)) ? (&PCICR) : ((uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? 2 : \
                                0 ) )

#define digitalPinToPCMSK(p)    ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? (&PCMSK0) : \
                                ( (((p) >= 62) && ((p) <= 69)) ? (&PCMSK2) : \
                                ((uint8_t *)0) ) )

#define digitalPinToPCMSKbit(p) ( (((p) >= 10) && ((p) <= 13)) ? ((p) - 6) : \
                                ( ((p) == 50) ? 3 : \
                                ( ((p) == 51) ? 2 : \
                                ( ((p) == 52) ? 1 : \
                                ( ((p) == 53) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? ((p) - 62) : \
                                0 ) ) ) ) ) )

#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) >= 18 && (p) <= 21 ? 23 - (p) : NOT_AN_INTERRUPT)))

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
	(uint16_t) &DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
	(uint16_t) &PINF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST		
	// -------------------------------------------		
	PE	, // PE 0 ** 0 ** USART0_RX	
	PE	, // PE 1 ** 1 ** USART0_TX	
	PE	, // PE 2 ** 2 ** 	
	PE	, // PE 3 ** 3 ** 	
	PE	, // PE 4 ** 4 ** SCL
	PE	, // PE 5 ** 5 ** SDA		
	PE	, // PE 6 ** 6 ** 	
	PE	, // PE 7 ** 7 ** 	
	PB	, // PB 0 ** 8 ** SS	
	PB	, // PB 1 ** 9 ** SCK	
	PB	, // PB 2 ** 10 ** MOSI	
	PB	, // PB 3 ** 11 ** MISO	
	PB	, // PB 4 ** 12 ** 	
	PB	, // PB 5 ** 13 ** 	
	PB	, // PB 6 ** 14 ** 	
	PB	, // PB 7 ** 15 ** 	
	PF	, // PF 0 ** 16 ** A0	
	PF	, // PF 1 ** 17 ** A1	
	PF	, // PF 2 ** 18 ** A2	
	PF	, // PF 3 ** 19 ** A3	
	PF	, // PF 4 ** 20 ** A4	
	PF	, // PF 5 ** 21 ** A5	
	PF	, // PF 6 ** 22 ** A6	
	PF	, // PF 7 ** 23 ** A7	
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PE 0 ** 0 ** USART0_RX	
	_BV( 1 )	, // PE 1 ** 1 ** USART0_TX	
	_BV( 2 )	, // PE 2 ** 2 ** 	
	_BV( 3 )	, // PE 3 ** 3 ** 	
	_BV( 4 )	, // PE 4 ** 4 ** SCL
	_BV( 5 )	, // PE 5 ** 5 ** SDA		
	_BV( 6 )	, // PE 6 ** 6 ** 	
	_BV( 7 )	, // PE 7 ** 7 ** 	
	_BV( 0 )	, // PB 0 ** 8 ** SS	
	_BV( 1 )	, // PB 1 ** 9 ** SCK	
	_BV( 2 )	, // PB 2 ** 10 ** MOSI	
	_BV( 3 )	, // PB 3 ** 11 ** MISO	
	_BV( 4 )	, // PB 4 ** 12 ** 	
	_BV( 5 )	, // PB 5 ** 13 ** 	
	_BV( 6 )	, // PB 6 ** 14 ** 	
	_BV( 7 )	, // PB 7 ** 15 ** 	
	_BV( 0 )	, // PF 0 ** 16 ** A0	
	_BV( 1 )	, // PF 1 ** 17 ** A1	
	_BV( 2 )	, // PF 2 ** 18 ** A2	
	_BV( 3 )	, // PF 3 ** 19 ** A3	
	_BV( 4 )	, // PF 4 ** 20 ** A4	
	_BV( 5 )	, // PF 5 ** 21 ** A5	
	_BV( 6 )	, // PF 6 ** 22 ** A6	
	_BV( 7 )	, // PF 7 ** 23 ** A7	
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	NOT_ON_TIMER	, // PE 0 ** 0 ** USART0_R
	NOT_ON_TIMER	, // PE 1 ** 1 ** USART0_T
	NOT_ON_TIMER	, // PE 2 ** 2 ** 	
	NOT_ON_TIMER	, // PE 3 ** 3 ** 	
	NOT_ON_TIMER	, // PE 4 ** 4 ** SCL
	NOT_ON_TIMER	, // PE 5 ** 5 ** SDA		
	NOT_ON_TIMER	, // PE 6 ** 6 ** 	
	NOT_ON_TIMER	, // PE 7 ** 7 ** 	
	NOT_ON_TIMER	, // PB 0 ** 8 ** SS	
	NOT_ON_TIMER	, // PB 1 ** 9 ** SCK	
	NOT_ON_TIMER	, // PB 2 ** 10 ** MOSI	
	NOT_ON_TIMER	, // PB 3 ** 11 ** MISO	
	NOT_ON_TIMER	, // PB 4 ** 12 ** 	
	NOT_ON_TIMER	, // PB 5 ** 13 ** 	
	NOT_ON_TIMER	, // PB 6 ** 14 ** 	
	NOT_ON_TIMER	, // PB 7 ** 15 ** 	
	NOT_ON_TIMER	, // PF 0 ** 16 ** A0	
	NOT_ON_TIMER	, // PF 1 ** 17 ** A1	
	NOT_ON_TIMER	, // PF 2 ** 18 ** A2	
	NOT_ON_TIMER	, // PF 3 ** 19 ** A3	
	NOT_ON_TIMER	, // PF 4 ** 20 ** A4	
	NOT_ON_TIMER	, // PF 5 ** 21 ** A5	
	NOT_ON_TIMER	, // PF 6 ** 22 ** A6	
	NOT_ON_TIMER	, // PF 7 ** 23 ** A7	
};

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif

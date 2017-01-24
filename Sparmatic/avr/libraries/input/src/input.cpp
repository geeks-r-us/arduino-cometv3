/// \file keys.c
///
///
///

/************************************************************************/
/*                                                                      */
/*                      Debouncing 8 Keys                               */
/*                      Sampling 4 Times                                */
/*                      With Repeat Function                            */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#include "input.h"

/// \brief .
///
///
void input::periodicScan()
{
    static uint8_t ct0, ct1, rpt;
    uint8_t i;
    int8_t newValue, diff;
#ifdef ENCODER
    newValue = 0;
    if (PHASE_A)
        newValue = 3;
    if (PHASE_B)
        newValue ^= 1;      // convert gray to binary
    diff = _last - newValue; // difference last - new
    if (diff & 1)
    {                                // bit 0 = value (1)
        _last = newValue;             // store new as next last
        _enc_delta += (diff & 2) - 1; // bit 1 = direction (+/-)
    }
#endif
	
    i = key_state ^ ~INPUT_PIN;   // key changed ?
    ct0 = ~(ct0 & i);           // reset or count ct0
    ct1 = ct0 ^ (ct1 & i);      // reset or count ct1
    i &= ct0 & ct1;             // count until roll over ?
    key_state ^= i;             // then toggle debounced state
    key_press |= key_state & i; // 0->1: key press detect

    if ((key_state & REPEAT_MASK) == 0) // check repeat function
        rpt = REPEAT_START;             // start delay
    if (--rpt == 0)
    {
        rpt = REPEAT_NEXT; // repeat delay
        key_rpt |= key_state & REPEAT_MASK;
    }
}

/// \brief .
///
///
///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed. Each pressed key is reported
// only once
//
uint8_t input::get_key_press(uint8_t key_mask)
{
    cli();                 // read and clear atomic !
    key_mask &= key_press; // read key(s)
    key_press ^= key_mask; // clear key(s)
    sei();
    return key_mask;
}

/// \brief .
///
///
///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported being pressed in subsequent calls
// to this function. This simulates the user repeatedly
// pressing and releasing the key.
//
uint8_t input::get_key_rpt(uint8_t key_mask)
{
    cli();               // read and clear atomic !
    key_mask &= key_rpt; // read key(s)
    key_rpt ^= key_mask; // clear key(s)
    sei();
    return key_mask;
}

/// \brief .
///
///
///////////////////////////////////////////////////////////////////
//
uint8_t input::get_key_short(uint8_t key_mask)
{
    cli(); // read key state and key press atomic !
    return get_key_press(~key_state & key_mask);
}

/// \brief .
///
///
///////////////////////////////////////////////////////////////////
//
uint8_t input::get_key_long(uint8_t key_mask)
{
    return get_key_press(get_key_rpt(key_mask));
}
#ifdef ENCODER
/// \brief .
///
///
/*
 * get increment/decrement keys.
 */
int8_t input::get_key_increment(void)
{

    int8_t keys = encoderRead();
    // limit
    if (keys > 0)
        return 1;
    if (keys < 0)
        return -1;

    return 0;
}
#endif 
/// \brief .
///
///
void input::init(void)
{
    // Configure capture routines
    INPUT_DDR &= ~INPUT_ALL; // configure port for input
    INPUT_PORT |= INPUT_ALL; // and turn on pull up resistors

    EIMSK |= (1 << PCIE1); //PC-INT 8..15
    PCMSK1 |= INPUT_ALL;   // Enable all switches PC-INT
#ifdef ENCODER 
    int8_t newValue = 0;
    if (PHASE_A)
        newValue = 3;
    if (PHASE_B)
        newValue ^= 1; // convert gray to binary
    _last = newValue;   // power on state
    _enc_delta = 0;
#endif
}

#ifdef ENCODER
int8_t input::encoderRead() // read two step encoders
{
    int8_t val;

    cli();
    val = _enc_delta;
    _enc_delta = val & 1;
    sei();
    return val >> 1;
}
#endif
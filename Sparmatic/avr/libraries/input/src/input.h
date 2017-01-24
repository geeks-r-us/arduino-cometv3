/// \file keys.h
/// 
/// 
/// 

/*
 * keys.h
 *
 *  Created on: 19.11.2011
 *      Author: matthias
 *      */
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

#ifndef INPUT_H_
#define INPUT_H_

#ifdef ENCODER
	#define ENCODER_A PB0
	#define ENCODER_B PB7
#else
	#define KEY_PLUS PB0
	#define KEY_MINUS PB7
#endif
#define KEY_CLOCK PB5
#define KEY_OK PB6
#define KEY_MENU PB4

#ifdef ENCODER 
	#define INPUT_ALL ((1<<ENCODER_A) | (1<<ENCODER_B) | (1 << KEY_CLOCK) | (1 << KEY_MENU) | (1 << KEY_OK))
#else 
	#define INPUT_ALL ((1<<KEY_PLUS) | (1<<KEY_MINUS) | (1 << KEY_CLOCK) | (1 << KEY_MENU) | (1 << KEY_OK))
#endif	

#define INPUT_DDR         DDRB
#define INPUT_PORT        PORTB
#define INPUT_PIN         PINB

#define KEY(x) (1 << x)

#ifdef ENCODER 
	#define PHASE_A     (INPUT_PIN & 1<<ENCODER_A)
	#define PHASE_B     (INPUT_PIN & 1<<ENCODER_B)
	#define REPEAT_MASK     ((1 << KEY_CLOCK) | (1 << KEY_MENU) | (1 << KEY_OK))   // repeat: all keys
#else
	#define REPEAT_MASK     ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_CLOCK) | (1 << KEY_MENU) | (1 << KEY_OK))   // repeat: all keys
#endif

#define REPEAT_START    30            // after N scans
#define REPEAT_NEXT     9             // every M scans

class input{
public:
    void init();
    void periodicScan();
    uint8_t get_key_press( uint8_t key_mask );
    uint8_t get_key_rpt( uint8_t key_mask );
    uint8_t get_key_short( uint8_t key_mask );
    uint8_t get_key_long( uint8_t key_mask );
#ifdef ENCODER
	int8_t get_key_increment();
	int8_t encoderRead();
#endif

private:
    uint8_t key_state;                                // debounced and inverted key state:
                                                      // bit = 1: key pressed
    uint8_t key_press;                                // key press detect

    uint8_t key_rpt;                                  // key long press and repeat
#ifdef ENCODER
    int8_t _enc_delta; // -128 ... 127
#endif
    int8_t _last;

    

};

static input Input;


#endif /* INPUT_H_ */
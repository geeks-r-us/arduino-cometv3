/// \file ntc.h
/// 
/// 
/// 

/*
 * ntc.h
 *
 *  Created on: 19.11.2011
 *      Author: matthias
 */

#ifndef NTC_H_
#define NTC_H_

#include <stdint.h>

class ntc {
public:    
    ntc();
    void init();
    void updateTemperature();
    int16_t getTemperature();
private:
    uint16_t getNtcAdc();

    int16_t _temperature;
    int8_t _offset;
};

static ntc NTC;



#define getNtcTemperature(x) ((const int16_t)Temperature)

#endif /* NTC_H_ */
/// \file motor.h
/// 
/// 
/// 

/*
 * motor.h
 *
 *  Created on: 19.11.2011
 *      Author: matthias
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

#define MOTOR_SENSE_PIN PE1


class motor {
public:    
    motor();
    void init();
    void stepOpen();
    void stepClose();
    uint8_t step();
    void moveTo(uint8_t valve);
    uint8_t fullOpen();
    uint8_t adapt();
    uint8_t isRunning();
    void timer();

private:
    uint16_t getCurrent();
    void stopMove();
    void stopTimeout();
    void move(int8_t dir);


    int16_t _position;
    int16_t _positionValveOpen;
    int16_t _positionValveClosed;
    uint8_t _stopSource;
    int8_t _direction;
    int16_t _targetPosition;
    uint8_t _timeout;
    uint16_t _currentLimit;
};

static motor Motor;
#define getMotorPosition(x) ((const volatile int16_t)MotorPosition)

#endif /* MOTOR_H_ */

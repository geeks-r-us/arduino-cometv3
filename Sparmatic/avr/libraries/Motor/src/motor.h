#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

#define MOTOR_SENSE_PIN PE1

enum MotorDir {
	CLOSE = -1,
	STOP = 0,
	OPEN = 1
};


class motor {
public:    
    motor();
    void init();
	void move(enum MotorDir direction);
private:
	void open();
	void close();
	void stop();
	
    
private:
	enum MotorDir m_direction;
};

static motor Motor;
#define getMotorPosition(x) ((const volatile int16_t)MotorPosition)

#endif /* MOTOR_H_ */


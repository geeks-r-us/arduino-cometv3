#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

enum MotorDir {
	CLOSE = 0,
	STOP = 1,
	OPEN = 2,
	FULLOPEN = 4,
	FULLCLOSE = 8
};


class motor {
public:
	volatile bool running = false;
	volatile int16_t position = 10000;				// ?? high value to avoid start up problems, should be private

	void init();
	void motorISR();
	void timer();
	void moveTo(uint8_t pos);

private:
	volatile int16_t target = 0;
	volatile int16_t valveClosedPosition = 1000;	// ?? MOTOR_MAX_CLOSE
	volatile int timerDelay = 3;					// delay for motor stop detection (MOTOR_TIMER_DELAY)

	void open();
	void close();
	void stop();
	uint16_t getCurrent();
	void endPositionReached();

//private:
public:
	enum MotorDir m_direction = STOP;
};

static motor Motor;

#endif /* MOTOR_H_ */

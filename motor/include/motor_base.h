#ifndef MOTOR_BASE_H
#define MOTOR_BASE_H

#include <motor_macros.h>
#include <vector>

class MOTOR_EXPORT IMotor
{
public:
	virtual ~IMotor() { }
	virtual void setup(const std::vector<int>& pins) = 0;
	virtual void move(bool clockwise) = 0;
	virtual void stop() = 0;
};

#endif

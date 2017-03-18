#ifndef MOTOR_BASE_H
#define MOTOR_BASE_H

#include <motor_macros.h>
#include <vector>
#include <stdint.h>

class MOTOR_EXPORT IMotor
{
public:
	virtual ~IMotor() { }
	virtual void setup(const std::vector<uint8_t>& pins) = 0;
	virtual void move(bool clockwise) = 0;
};

#endif

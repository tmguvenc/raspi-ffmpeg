#ifndef STEP_MOTOR_28BYJ48_H
#define STEP_MOTOR_28BYJ48_H

#include <motor_base.h>
#include <functional>
#include <atomic>

class MOTOR_EXPORT StepMotor28BYJ48 : public IMotor
{
public:
	StepMotor28BYJ48();
	~StepMotor28BYJ48();
	void setup(const std::vector<int>& pins) override;
	void move(bool clockwise) override;
	void stop() override;

protected:
	void setStep(int pin1, int pin2, int pin3, int pin4);
	void clockwiseSingleStep(int ms);
	void counterClockwiseSingleStep(int ms);

private:
	std::vector<std::vector<int>> m_array;
	int m_pins[4];
	std::atomic<bool> m_move;

	std::vector<std::function<void(int)>> m_funcs;
};

#endif

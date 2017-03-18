#ifndef STEP_MOTOR_28BYJ48_H
#define STEP_MOTOR_28BYJ48_H

#include <motor_base.h>
#include <functional>
#include <atomic>

class MOTOR_EXPORT StepMotor28BYJ48 : public IMotor
{
public:
	explicit StepMotor28BYJ48(uint32_t delayMicroseconds, uint32_t step);
	~StepMotor28BYJ48();
	void setup(const std::vector<uint8_t>& pins) override;
	void move(bool clockwise) override;

protected:
	void setStep(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
	void clockwiseSingleStep(uint32_t ms);
	void counterClockwiseSingleStep(uint32_t ms);

private:
	std::vector<std::vector<uint8_t>> m_array;
	int m_pins[4];
	std::atomic<bool> m_move;

	std::vector<std::function<void(uint32_t)>> m_funcs;
	uint32_t m_step;
	uint32_t m_delayMicroseconds;
};

#endif

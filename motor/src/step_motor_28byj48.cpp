#include <step_motor_28byj48.h>
#include <algorithm>
#include "rgpio.h"

StepMotor28BYJ48::StepMotor28BYJ48(uint32_t delayMicroseconds, uint32_t step) :
m_step(step),
m_delayMicroseconds(delayMicroseconds)
{
	m_array =
	{
		{ 1, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 1 },
		{ 1, 0, 0, 1 }
	};

	m_funcs = {
		[this](uint32_t ms){this->counterClockwiseSingleStep(ms); },
		[this](uint32_t ms){this->clockwiseSingleStep(ms); }
	};
}

StepMotor28BYJ48::~StepMotor28BYJ48()
{
}

void StepMotor28BYJ48::setup(const std::vector<uint8_t>& pins)
{
	if (pins.size() != 4)
		throw std::invalid_argument("Invalid Pin Count");
	std::copy(pins.begin(), pins.end(), std::begin(m_pins));

	for (auto pin : m_pins)
		GPIO::setPinMode(pin, OUTPUT);
}

void StepMotor28BYJ48::move(bool clockwise)
{
	for (uint32_t i = 0; i < m_step; ++i)
		m_funcs[clockwise](m_delayMicroseconds);

	setStep(0, 0, 0, 0);
}

void StepMotor28BYJ48::setStep(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
{
	GPIO::write(m_pins[0], pin1);
	GPIO::write(m_pins[1], pin2);
	GPIO::write(m_pins[2], pin3);
	GPIO::write(m_pins[3], pin4);
}

// 0.703125 degree step
void StepMotor28BYJ48::clockwiseSingleStep(uint32_t ms) {
	for (auto i = 0; i < 8; ++i){
		setStep(m_array[i][0], m_array[i][1], m_array[i][2], m_array[i][3]);
		GPIO::sleepMicroSeconds(ms);
	}
}

void StepMotor28BYJ48::counterClockwiseSingleStep(uint32_t ms)
{
	for (auto i = 7; i >= 0; --i){
		setStep(m_array[i][0], m_array[i][1], m_array[i][2], m_array[i][3]);
		GPIO::sleepMicroSeconds(ms);
	}
}

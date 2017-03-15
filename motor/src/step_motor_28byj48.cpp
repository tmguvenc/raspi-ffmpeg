#include <step_motor_28byj48.h>
#include <algorithm>
#include "rgpio.h"

StepMotor28BYJ48::StepMotor28BYJ48()
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
		[this](int ms){this->counterClockwiseSingleStep(ms); },
		[this](int ms){this->clockwiseSingleStep(ms); }
	};
}

StepMotor28BYJ48::~StepMotor28BYJ48()
{

}

void StepMotor28BYJ48::setup(const std::vector<int>& pins)
{
	if (pins.size() != 4)
		throw std::invalid_argument("Invalid Pin Count");
	std::copy(pins.begin(), pins.end(), std::begin(m_pins));

	for (auto pin : m_pins)
		GPIO::setPinMode(pin, OUTPUT);
}

void StepMotor28BYJ48::move(bool clockwise)
{
	for(int i = 0; i < 8; ++i)
		m_funcs[clockwise](1200);
}

void StepMotor28BYJ48::stop()
{
	setStep(0, 0, 0, 0);
}

void StepMotor28BYJ48::setStep(int pin1, int pin2, int pin3, int pin4)
{
	GPIO::write(m_pins[0], pin1);
	GPIO::write(m_pins[1], pin2);
	GPIO::write(m_pins[2], pin3);
	GPIO::write(m_pins[3], pin4);
}

// 0.703125 degree step
void StepMotor28BYJ48::clockwiseSingleStep(int ms) {
	for (auto i = 0; i < 8; ++i){
		setStep(m_array[i][0], m_array[i][1], m_array[i][2], m_array[i][3]);
		GPIO::sleepMicroSeconds(ms);
	}
}

void StepMotor28BYJ48::counterClockwiseSingleStep(int ms)
{
	for (auto i = 7; i >= 0; --i){
		setStep(m_array[i][0], m_array[i][1], m_array[i][2], m_array[i][3]);
		GPIO::sleepMicroSeconds(ms);
	}
}

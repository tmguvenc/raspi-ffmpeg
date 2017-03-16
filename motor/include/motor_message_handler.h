#ifndef MOTOR_MESSAGE_HANDLER_H
#define MOTOR_MESSAGE_HANDLER_H

#include <message_handler.h>
#include <step_motor_28byj48.h>
#include <memory>
#include <messages.h>

class MotorMessageHandler
{
public:
	MotorMessageHandler(int delayMicroseconds, int step, const std::vector<int>& panPins, const std::vector<int>& tiltPins)
	{
		m_panMotor = std::make_unique<StepMotor28BYJ48>(delayMicroseconds, step);
		m_panMotor->setup(std::move(panPins));

		m_tiltMotor = std::make_unique<StepMotor28BYJ48>(delayMicroseconds, step);
		m_tiltMotor->setup(std::move(tiltPins));

		m_functionMap.resize(14, nullptr);
		m_functionMap[MotorRightRequest] = [this](){m_panMotor->move(true); };
		m_functionMap[MotorLeftRequest] = [this](){m_panMotor->move(false); };
		m_functionMap[MotorUpRequest] = [this](){m_tiltMotor->move(true); };
		m_functionMap[MotorDownRequest] = [this](){m_tiltMotor->move(false); };
	}

	Data* execute(const MessageType& message)
	{
		assert(m_functionMap[message] != nullptr && "Invalid motor function request");
		m_functionMap[message]();
		return nullptr;
	}

private:
	std::unique_ptr<IMotor> m_panMotor;
	std::unique_ptr<IMotor> m_tiltMotor;
	using Function = std::function<void()>;

	std::vector<Function> m_functionMap;
};

#endif
#ifndef MESSAGES_H
#define MESSAGES_H

enum MessageType
{
	FrameRequest = 0,
	StopRequest = 1,
	HumTempRequest = 2,
	InitRequest = 3,
	MotorUpRequest = 4,
	MotorDownRequest = 5,
	MotorRightRequest = 6,
	MotorLeftRequest = 7,
	Ping = 8,
	InitResponse = 9,
	FrameResponse = 10,
	StopResponse = 11,
	HumTempResponse = 12,
	StopQueueMessage = 13,
	MessageCount = 14
};

#endif
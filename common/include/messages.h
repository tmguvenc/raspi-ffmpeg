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
	MotorStopRequest = 8,
	Ping = 9,
	InitResponse = 10,
	FrameResponse = 11,
	StopResponse = 12,
	HumTempResponse = 13
};

#endif
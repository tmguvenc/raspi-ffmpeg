#ifndef MESSAGES_H
#define MESSAGES_H

enum MessageType : unsigned char
{
	VideoFrameRequest = 0,
	AudioFrameRequest = 1,
	StopRequest = 2,
	HumTempRequest = 3,
	InitRequest = 4,
	MotorUpRequest = 5,
	MotorDownRequest = 6,
	MotorRightRequest = 7,
	MotorLeftRequest = 8,
	Ping = 9,
	InitResponse = 10,
	VideoFrameResponse = 11,
	AudioFrameResponse = 12,
	StopResponse = 13,
	HumTempResponse = 14,
	StopQueueMessage = 15,
	MessageCount = 16
};

#endif
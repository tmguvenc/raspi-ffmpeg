#ifndef MESSAGES_H
#define MESSAGES_H

enum MessageType
{
	FrameRequest = 0,
	StopRequest = 1,
	HumTempRequest = 2,
	InitRequest = 3,
	Ping = 4,
	InitResponse = 5,
	FrameResponse = 6,
	StopResponse = 7,
	HumTempResponse = 8
};

#endif
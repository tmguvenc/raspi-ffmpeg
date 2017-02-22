#ifndef MESSAGE_H
#define MESSAGE_H

struct Message
{
	virtual ~Message() { }
	int type;
};

struct PingMessage : Message
{
	PingMessage()
	{
		type = 0;
	}
};

struct FrameRequestMessage : Message
{
	FrameRequestMessage()
	{
		type = 1;
	}
};

#endif
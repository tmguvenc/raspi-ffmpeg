#ifndef RECEIVE_STRATEGY_H
#define RECEIVE_STRATEGY_H

class Data;

class IReceiveStrategy{
public:
	virtual ~IReceiveStrategy() { }
	virtual void handle(Data* data) = 0;
};

#endif
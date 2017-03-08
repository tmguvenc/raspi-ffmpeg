#ifndef RECEIVE_STRATEGY_QUEUE_H
#define RECEIVE_STRATEGY_QUEUE_H

#include <receive_strategy.h>
#include <stdexcept>

template<typename Queue>
class ReceiveStrategyQueue : public IReceiveStrategy{
public:
	explicit ReceiveStrategyQueue(Queue* queue) : 
		m_queue(queue)
	{
		if (m_queue == nullptr){
			throw std::invalid_argument("receive queue is null");
		}
	}

	~ReceiveStrategyQueue() override
	{
		while (!m_queue->empty()) {
			typename Queue::value_type data;
			m_queue->pop(data);
		}
	}

	virtual void handle(Data* data) override
	{
		m_queue->push(data);
	}

private:
	Queue* m_queue;
};

#endif
#ifndef RECEIVE_STRATEGY_QUEUE_H
#define RECEIVE_STRATEGY_QUEUE_H

#include <receive_strategy.h>
#include <stdexcept>
#include <type_traits>

template<typename Queue>
class ReceiveStrategyQueue : public IReceiveStrategy{
public:
	using T = typename Queue::value_type;

	explicit ReceiveStrategyQueue(Queue* queue) : 
		m_queue(queue)
	{
		static_assert(std::is_pointer<T>::value, "ReceiveStrategyQueue requires pointer type as queue element.");
		
		if (m_queue == nullptr){
			throw std::invalid_argument("receive queue is null");
		}
	}

	~ReceiveStrategyQueue() override
	{
		while (!m_queue->empty()) {
			T data;
			m_queue->pop(data);
			delete data;
		}
	}

	virtual void handle(T data) override
	{
		m_queue->push(data);
	}

private:
	Queue* m_queue;
};

#endif
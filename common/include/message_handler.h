#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <tbb/tbb_thread.h>
#include <memory>
#include <atomic>

template<typename TData, typename TMessage>
class Handler
{
public:
	std::unique_ptr<TData> execute(const TMessage&) { return nullptr; }
};

template<typename RequestQueue, typename ResponseQueue, typename Executer>
class MessageHandler
{
	using RequestValueType = typename RequestQueue::value_type;
	using ResponseValueType = typename ResponseQueue::value_type;
public:
	explicit MessageHandler(RequestQueue* requestQueue, ResponseQueue* responseQueue, Executer* executer) :
		m_run(false), 
		m_requestQueue(requestQueue),
		m_resposeQueue(responseQueue),
		m_executer(executer)
	{
		
	}
	~MessageHandler()
	{
		stop();

		if (m_thread->joinable())
			m_thread->join();
	}

	void start()
	{
		m_thread = std::make_unique<tbb::tbb_thread>([]()
		{
			m_run.store(true);
			while (m_run)
			{
				RequestValueType request;
				m_requestQueue->pop(request);
				auto retVal = m_executer->execute(request);
				
				static_assert(std::is_same<decltype(retVal), ResponseValueType>::value, "retval must be ResponseValueType");

				if (m_resposeQueue != nullptr)
					m_resposeQueue->push(retVal);
			}
		});
	}

	void stop()
	{
		m_run.store(false);
	}
private:
	std::atomic<bool> m_run;
	std::unique_ptr<tbb::tbb_thread> m_thread;
	RequestQueue* m_requestQueue;
	ResponseQueue* m_resposeQueue;
	Executer *m_executer;
};

#endif
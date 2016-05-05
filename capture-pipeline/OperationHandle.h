/*
 * OperationHandle.h
 *
 *  Created on: 5 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef OPERATIONHANDLE_H_
#define OPERATIONHANDLE_H_
#include <memory>

namespace std{
	class mutex;
	class condition_variable;
}

class OperationHandle
{
private:
	bool m_complete;
	std::shared_ptr<std::mutex> m_mutex;
	std::shared_ptr<std::condition_variable> m_conditionVariable;
public:
	OperationHandle();
	OperationHandle(const OperationHandle& other);
	OperationHandle& operator=(const OperationHandle& other);
	~OperationHandle();
	void Wait();
	void Done();
};

#endif /* OPERATIONHANDLE_H_ */

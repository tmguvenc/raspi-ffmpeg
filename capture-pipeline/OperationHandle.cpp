/*
 * OperationHandle.cpp
 *
 *  Created on: 5 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "OperationHandle.h"
#include <mutex>
#include <condition_variable>

using namespace std;

OperationHandle::OperationHandle()
{
	m_complete = false;
	m_mutex = shared_ptr<mutex>(new mutex());
	m_conditionVariable = shared_ptr<condition_variable>(new condition_variable());
}

OperationHandle::OperationHandle(const OperationHandle& other)
{
	m_mutex = other.m_mutex;
	m_complete = other.m_complete;
	m_conditionVariable = other.m_conditionVariable;
}

OperationHandle& OperationHandle::operator=(const OperationHandle& other)
{
	m_mutex = other.m_mutex;
	m_complete = other.m_complete;
	m_conditionVariable = other.m_conditionVariable;
	return *this;
}

OperationHandle::~OperationHandle()
{

}

void OperationHandle::Wait()
{
	unique_lock<mutex> lock(*m_mutex);
	if (m_complete)
		return;
	m_conditionVariable->wait(lock);
}

void OperationHandle::Done()
{
	bool notify = false;
	unique_lock<mutex> lock(*m_mutex);
	if (!m_complete)
	{
		m_complete = true;
		notify = true;
	}
	lock.unlock();
	if (notify)
		m_conditionVariable->notify_all();
}

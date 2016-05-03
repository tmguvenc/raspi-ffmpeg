/*
 * ContextPool.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "ContextPool.h"
#include "FrameContext.h"
#include <chrono>
#include <thread>

ContextPool::ContextPool(size_t size) :
		m_contextPool("context", "Context Pool", [this]()
		{
			return spFrameContext(new FrameContext());
		}, size)
{
}

ContextPool::~ContextPool()
{

}

spFrameContext ContextPool::CreateFrameContext()
{
	return m_contextPool.create();
}

spFrameContext ContextPool::TryCreateFrameContext()
{
	return m_contextPool.tryCreate();
}

spFrameContext ContextPool::TryCreateFrameContext(size_t wait)
{
	auto start = std::chrono::system_clock::now();
	while (true)
	{
		auto current = std::chrono::system_clock::now();
		size_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(
				current - start).count();
		auto ctx = TryCreateFrameContext();
		if (ctx)
			return ctx;
		if (wait < diff)
			return ctx;
		//Sleep(20);// Why 20?

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

	}
}

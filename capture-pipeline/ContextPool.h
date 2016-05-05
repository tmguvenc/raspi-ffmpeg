/*
 * ContextPool.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef CONTEXTPOOL_H_
#define CONTEXTPOOL_H_

#include "ObjectPool.h"
#include "FrameContext.h"
#include <memory>

class ContextPoolCleanup
{
public:
	static void Push(FrameContext* context){

	}
	static void Pull(FrameContext* context){
		context->m_operationHandle.Done();
	}
};

class ContextPool
{
public:
	ContextPool(size_t size);
	virtual ~ContextPool();

	spFrameContext CreateFrameContext();
	spFrameContext TryCreateFrameContext();
	spFrameContext TryCreateFrameContext(size_t wait);

	double getBufferSize() { return m_contextPool.getRatio(); }

private:
	object_pool<FrameContext, ContextPoolCleanup> m_contextPool;
};

#endif /* CONTEXTPOOL_H_ */

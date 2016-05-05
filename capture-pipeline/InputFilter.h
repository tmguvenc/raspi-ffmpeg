/*
 * InputFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef INPUTFILTER_H_
#define INPUTFILTER_H_

#include "Filter.h"
#include <tbb/concurrent_queue.h>
#include "FrameContext.h"

class InputFilter: public Filter
{
public:
	InputFilter(tbb::concurrent_bounded_queue<spFrameContext>* queue);
	virtual ~InputFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;

private:
	tbb::concurrent_bounded_queue<spFrameContext>* m_queue;
};

#endif /* INPUTFILTER_H_ */

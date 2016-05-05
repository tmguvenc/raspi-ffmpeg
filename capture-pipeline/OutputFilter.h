/*
 * OutputFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef OUTPUTFILTER_H_
#define OUTPUTFILTER_H_

#include "Filter.h"
#include <tbb/concurrent_queue.h>
#include "FrameContext.h"

class OutputFilter: public Filter
{
public:
	OutputFilter(tbb::concurrent_bounded_queue<spFrameContext>* output_queue);
	virtual ~OutputFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;

private:
	tbb::concurrent_bounded_queue<spFrameContext>* m_output_queue;
};

#endif /* OUTPUTFILTER_H_ */

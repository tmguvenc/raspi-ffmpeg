/*
 * Filter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <tbb/pipeline.h>

class Filter : public tbb::filter
{
public:
	explicit Filter(mode mode) : tbb::filter(mode) { }

	virtual ~Filter() { }

	virtual void *operator()(void *) override
	{ return nullptr; }
	virtual void finalize(void *) override
	{ }
};

#endif /* FILTER_H_ */

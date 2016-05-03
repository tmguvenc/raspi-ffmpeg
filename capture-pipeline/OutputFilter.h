/*
 * OutputFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef OUTPUTFILTER_H_
#define OUTPUTFILTER_H_

#include "Filter.h"

class OutputFilter: public Filter
{
public:
	OutputFilter();
	virtual ~OutputFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;
};

#endif /* OUTPUTFILTER_H_ */

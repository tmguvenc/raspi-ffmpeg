/*
 * GrayscaleFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef GRAYSCALEFILTER_H_
#define GRAYSCALEFILTER_H_

#include "Filter.h"

class GrayscaleFilter: public Filter
{
public:
	GrayscaleFilter();
	virtual ~GrayscaleFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;
};

#endif /* GRAYSCALEFILTER_H_ */

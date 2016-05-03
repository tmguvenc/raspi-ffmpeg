/*
 * DecodeFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef DECODEFILTER_H_
#define DECODEFILTER_H_

#include "Filter.h"
#include "Decoder.h"

class DecodeFilter : public Filter
{
public:
	DecodeFilter();
	virtual ~DecodeFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;

private:
	Decoder decoder;
};

#endif /* DECODEFILTER_H_ */

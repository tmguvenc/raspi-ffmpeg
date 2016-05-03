/*
 * CaptureFilter.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef CAPTUREFILTER_H_
#define CAPTUREFILTER_H_

#include "Filter.h"
#include <string>
#include "ContextPool.h"

class WebcamCapture;

class CaptureFilter : public Filter
{
public:
	CaptureFilter(const std::string& connectionString);
	virtual ~CaptureFilter();

	virtual void *operator()(void *) override;
	virtual void finalize(void *) override;

private:
	WebcamCapture *m_capture;
	ContextPool m_contextPool;
};

#endif /* CAPTUREFILTER_H_ */

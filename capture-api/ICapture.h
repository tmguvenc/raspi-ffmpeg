/*
 * ICapture.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTURE_H_
#define ICAPTURE_H_

typedef void (*CaptureCallback)(void*);

struct CaptureSettings;

class ICapture {
public:
	virtual ~ICapture() {
	}
	virtual void init(CaptureSettings* settings) = 0;
	virtual void start(CaptureCallback func) = 0;
	virtual void startAsync(CaptureCallback func) = 0;
	virtual void stop() = 0;
	virtual bool completed() = 0;
};

#endif /* ICAPTURE_H_ */

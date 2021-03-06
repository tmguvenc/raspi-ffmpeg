/*
 * ICapture.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef ICAPTURE_H_
#define ICAPTURE_H_

typedef void (*CaptureCallback)(void*);

class ICapture {
public:
	virtual ~ICapture() {
	}
	virtual void init() = 0;
	virtual void start(CaptureCallback func) = 0;
	virtual void stop() = 0;
	virtual void teardown() = 0;
};

#endif /* ICAPTURE_H_ */

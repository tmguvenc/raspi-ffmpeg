/*
 * capture_base.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTURE_H_
#define ICAPTURE_H_

#include <capture_macros.h>
#include <functional>

class FrameContainer;

typedef std::function<void(FrameContainer*)> CaptureCallback;

class CaptureSettings;

class CAPTURE_EXPORT ICapture {
public:
	virtual ~ICapture() {
	}
	virtual void init(CaptureSettings* settings) = 0;
	virtual void start(CaptureCallback func) = 0;
	virtual void startAsync(CaptureCallback func) = 0;
	virtual void stop() = 0;
	virtual bool started() = 0;
	virtual bool completed() = 0;
	virtual void* getCodecInfo() = 0;
	virtual CaptureSettings* getSettings() = 0;

protected:
	CaptureSettings* m_settings;
};

#endif /* ICAPTURE_H_ */

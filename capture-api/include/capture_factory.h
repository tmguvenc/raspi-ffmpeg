/*
 * ICaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ICAPTUREFACTORY_H_
#define ICAPTUREFACTORY_H_

#include <capture_macros.h>
#include <string>
#include <memory>
#include <assert.h>
class ICapture;

template<typename Capture>
struct CaptureStopper
{
	void operator()(Capture* cap){
		assert(cap != nullptr && "capture is null");
		cap->stop();
	}
};

using upCapture = std::unique_ptr<ICapture, CaptureStopper<ICapture>>;

class CAPTURE_EXPORT ICaptureFactory {
public:
	virtual ~ICaptureFactory() { }
	virtual upCapture create(const std::string& connectionString) = 0;
};

#endif /* ICAPTUREFACTORY_H_ */

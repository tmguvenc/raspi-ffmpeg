/*
 * WebcamCaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef WEBCAMCAPTUREFACTORY_H_
#define WEBCAMCAPTUREFACTORY_H_

#include "capture_factory.h"

class CAPTURE_EXPORT WebcamCaptureFactory: public ICaptureFactory {
public:
	WebcamCaptureFactory(){}
	~WebcamCaptureFactory() {}
	ICapture* create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger) override;
};

#endif /* WEBCAMCAPTUREFACTORY_H_ */

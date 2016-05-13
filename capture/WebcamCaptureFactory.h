/*
 * WebcamCaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef WEBCAMCAPTUREFACTORY_H_
#define WEBCAMCAPTUREFACTORY_H_

#include "ICaptureFactory.h"

class WebcamCaptureFactory: public ICaptureFactory {
public:
	WebcamCaptureFactory(){}
	~WebcamCaptureFactory() {}
	ICapture* createFactory(const std::string& connectionString) override;
};

#endif /* WEBCAMCAPTUREFACTORY_H_ */

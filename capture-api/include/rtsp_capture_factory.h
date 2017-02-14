/*
 * RTSPCaptureFactory.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef RTSPCAPTUREFACTORY_H_
#define RTSPCAPTUREFACTORY_H_

#include <capture_factory.h>

class CAPTURE_EXPORT RTSPCaptureFactory: public ICaptureFactory {
public:
	RTSPCaptureFactory();
	virtual ~RTSPCaptureFactory();
	ICapture* create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger) override;
};

#endif /* RTSPCAPTUREFACTORY_H_ */

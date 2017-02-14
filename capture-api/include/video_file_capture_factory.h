/*
 * VideoFileCaptureFactory.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VIDEOFILECAPTUREFACTORY_H_
#define VIDEOFILECAPTUREFACTORY_H_

#include "capture_factory.h"

class CAPTURE_EXPORT VideoFileCaptureFactory: public ICaptureFactory {
public:
	VideoFileCaptureFactory();
	virtual ~VideoFileCaptureFactory();
	ICapture* create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger) override;
};

#endif /* VIDEOFILECAPTUREFACTORY_H_ */

/*
 * AudioCaptureFactory.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef AUDIO_CAPTURE_FACTORY_H_
#define AUDIO_CAPTURE_FACTORY_H_

#include "capture_factory.h"

class CAPTURE_EXPORT AudioCaptureFactory: public ICaptureFactory {
public:
	AudioCaptureFactory(){}
	~AudioCaptureFactory() {}
	upCapture create(const std::string& connectionString) override;
};

#endif /* AUDIO_CAPTURE_FACTORY_H_ */

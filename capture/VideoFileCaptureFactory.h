/*
 * VideoFileCaptureFactory.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef VIDEOFILECAPTUREFACTORY_H_
#define VIDEOFILECAPTUREFACTORY_H_

#include "ICaptureFactory.h"

class VideoFileCaptureFactory: public ICaptureFactory {
public:
	VideoFileCaptureFactory();
	virtual ~VideoFileCaptureFactory();
	ICapture* createFactory(const std::string& connectionString) override;
};

#endif /* VIDEOFILECAPTUREFACTORY_H_ */

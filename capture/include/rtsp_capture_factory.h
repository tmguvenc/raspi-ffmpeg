/*
 * RTSPCaptureFactory.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef RTSPCAPTUREFACTORY_H_
#define RTSPCAPTUREFACTORY_H_

#include <capture_factory.h>

class CAPTURE_EXPORT RTSPCaptureFactory: public ICaptureFactory {
public:
	RTSPCaptureFactory();
	virtual ~RTSPCaptureFactory();
	upCapture create(const std::string& connectionString) override;
};

#endif /* RTSPCAPTUREFACTORY_H_ */
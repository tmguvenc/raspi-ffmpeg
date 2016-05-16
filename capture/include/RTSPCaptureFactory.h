/*
 * RTSPCaptureFactory.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef RTSPCAPTUREFACTORY_H_
#define RTSPCAPTUREFACTORY_H_

#include "ICaptureFactory.h"

class RTSPCaptureFactory: public ICaptureFactory {
public:
	RTSPCaptureFactory();
	virtual ~RTSPCaptureFactory();
	ICapture* createFactory(const std::string& connectionString) override;
};

#endif /* RTSPCAPTUREFACTORY_H_ */

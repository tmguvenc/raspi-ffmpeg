/*
 * WebcamCaptureFactory.cpp
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat G�ven�
 */

#include "WebcamCaptureFactory.h"
#include "WebcamCapture.h"

ICapture* WebcamCaptureFactory::createFactory(const std::string& connectionString){
	return new WebcamCapture(connectionString);
}

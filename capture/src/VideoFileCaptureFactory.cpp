/*
 * VideoFileCaptureFactory.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "VideoFileCaptureFactory.h"
#include "VideoFileCapture.h"

VideoFileCaptureFactory::VideoFileCaptureFactory() {
}

VideoFileCaptureFactory::~VideoFileCaptureFactory() {

}

ICapture* VideoFileCaptureFactory::createFactory(const std::string& connectionString){
	return new VideoFileCapture(connectionString);
}

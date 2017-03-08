/*
 * WebcamCaptureFactory.cpp
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <string>
#include <webcam_capture_factory.h>
#include <webcam_capture.h>

upCapture WebcamCaptureFactory::create(const std::string& connectionString){
	auto aa = new WebcamCapture(std::move(connectionString));
	return upCapture(aa);
}

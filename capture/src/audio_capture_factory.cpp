/*
 * AudioCaptureFactory.cpp
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <string>
#include <audio_capture_factory.h>
#include <audio_capture.h>

upCapture AudioCaptureFactory::create(const std::string& connectionString){
	auto aa = new AudioCapture(std::move(connectionString));
	return upCapture(aa);
}

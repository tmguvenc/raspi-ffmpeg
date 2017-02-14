/*
 * WebcamCaptureFactory.cpp
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <string>
#include <webcam_capture_factory.h>
#include <webcam_capture.h>
#include <spdlog/spdlog.h>

ICapture* WebcamCaptureFactory::create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger){
	return new WebcamCapture(std::move(connectionString), std::forward<std::shared_ptr<spdlog::logger>>(logger));
}

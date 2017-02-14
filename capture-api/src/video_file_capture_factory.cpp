/*
 * VideoFileCaptureFactory.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <video_file_capture_factory.h>
#include <capture_base.h>
#include <string>
#include <video_file_capture.h>
#include <spdlog/spdlog.h>

VideoFileCaptureFactory::VideoFileCaptureFactory() {
}

VideoFileCaptureFactory::~VideoFileCaptureFactory() {

}

ICapture* VideoFileCaptureFactory::create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger){
	return new VideoFileCapture(std::move(connectionString), std::forward<std::shared_ptr<spdlog::logger>>(logger));
}

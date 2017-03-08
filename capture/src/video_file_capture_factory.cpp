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

upCapture VideoFileCaptureFactory::create(const std::string& connectionString){
	auto aa = new VideoFileCapture(std::move(connectionString));
	return upCapture(aa);
}

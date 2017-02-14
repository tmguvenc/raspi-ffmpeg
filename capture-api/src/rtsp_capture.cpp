/*
 * RTSPCapture.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <rtsp_capture.h>
#include <string>

RTSPCapture::RTSPCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger) :
VideoFileCapture(std::move(connectionString), std::forward<std::shared_ptr<spdlog::logger>>(logger)) {

}

RTSPCapture::~RTSPCapture() {

}


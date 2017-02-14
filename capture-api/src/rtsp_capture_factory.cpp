/*
 * RTSPCaptureFactory.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <rtsp_capture_factory.h>
#include <rtsp_capture.h>

RTSPCaptureFactory::RTSPCaptureFactory() {
	// TODO Auto-generated constructor stub

}

RTSPCaptureFactory::~RTSPCaptureFactory() {
	// TODO Auto-generated destructor stub
}

ICapture* RTSPCaptureFactory::create(const std::string& connectionString, std::shared_ptr<spdlog::logger>&& logger){
	return new RTSPCapture(std::move(connectionString), std::forward<std::shared_ptr<spdlog::logger>>(logger));
}

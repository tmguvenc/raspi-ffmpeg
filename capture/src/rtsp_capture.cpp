/*
 * RTSPCapture.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat G�ven�
 */

#include <rtsp_capture.h>
#include <string>

RTSPCapture::RTSPCapture(const std::string& connectionString) :
VideoFileCapture(std::move(connectionString)) {

}

RTSPCapture::~RTSPCapture() {

}


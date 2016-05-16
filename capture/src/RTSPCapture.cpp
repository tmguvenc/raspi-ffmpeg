/*
 * RTSPCapture.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "RTSPCapture.h"

RTSPCapture::RTSPCapture(const std::string& connectionString) : VideoFileCapture(connectionString) {

}

RTSPCapture::~RTSPCapture() {

}


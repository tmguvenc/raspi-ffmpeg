/*
 * main.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <iostream>
#include <capture/capture.h>

const auto WIDTH = 1024;
const auto HEIGHT = 768;

int main() {
	
	ICaptureFactory* captureFactory = new WebcamCaptureFactory();
	auto capture = captureFactory->createFactory("/dev/video0");

	capture->init(nullptr);

	capture->stop();

	delete capture;
	delete captureFactory;

	return 0;
}


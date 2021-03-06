/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#include <zmq.h>
#include <iostream>
#include "WebcamCapture.h"
#include <csignal>
#include <memory>
#include <tbb/concurrent_queue.h>
#include "Decoder.h"
#include <opencv2/opencv.hpp>
#include "VAFrame.h"

#define WIDTH 640
#define HEIGHT 480

tbb::concurrent_bounded_queue<void*> frameQueue;
ICapture *capture;

int main()
{
	frameQueue.set_capacity(20);

	capture = new WebcamCapture("/dev/video0");

	capture->init();
	capture->start([](void* ptr)
	{
		frameQueue.push(ptr);
	});

	Decoder decoder;
	decoder.setup(AV_CODEC_ID_MJPEG);

	const auto len = HEIGHT * WIDTH * 3;
	char buffer[len] = { 0 };

	cv::namedWindow("image");

	while(true){
		void* ptr = nullptr;
		frameQueue.pop(ptr);

		if(ptr != nullptr){
			VAFrame* frame = static_cast<VAFrame*>(ptr);

			if (frame->size() == 0) break;

			decoder.decode(frame->data(), frame->size(), buffer, len);

			cv::Mat image(HEIGHT, WIDTH, CV_8UC3, buffer);

			cv::imshow("image", image);

			if (cv::waitKey(40) == 27) break;

			delete frame;
		}
	}

	cv::destroyAllWindows();

	capture->teardown();
	delete capture;

	return 0;
}

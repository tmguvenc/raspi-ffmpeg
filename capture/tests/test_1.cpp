/*
 * main.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <iostream>
#include <tbb/concurrent_queue.h>
#include <opencv2/opencv.hpp>
#include <capture/capture.h>

const auto WIDTH = 1024;
const auto HEIGHT = 768;

tbb::concurrent_bounded_queue<void*> frameQueue;

int main() {

	frameQueue.set_capacity(20);

	//ICaptureFactory* captureFactory = new RTSPCaptureFactory();
	//auto capture = captureFactory->createFactory("rtsp://admin:12345@192.168.1.68/stream1");

	//ICaptureFactory* captureFactory = new VideoFileCaptureFactory();
	//auto capture = captureFactory->createFactory("/home/pi/Desktop/output_2014117_838_1.avi");

	ICaptureFactory* captureFactory = new WebcamCaptureFactory();
	auto capture = captureFactory->createFactory("/dev/video0");

	CaptureSettings set(WIDTH, HEIGHT, 3, 5, AV_CODEC_ID_MJPEG);

	capture->init(&set);

	Decoder decoder(WIDTH, HEIGHT);

	if (!decoder.setup(capture)) {
		std::cout << "cannot initialize decoder" << std::endl;
		exit(EXIT_FAILURE);
	}

	capture->startAsync([](void* ptr) {
		frameQueue.push(ptr);
	});

	const auto len = WIDTH * HEIGHT * 3;
	char buffer[len] = { 0 };

	cv::namedWindow("image");
	cv::moveWindow("image", 200, 200);

	while (true) {
		void* ptr = nullptr;
		frameQueue.pop(ptr);

		if (ptr != nullptr) {
			VAFrameContainer* frame = static_cast<VAFrameContainer*>(ptr);
			auto data = frame->data();
			auto size = frame->size();

			if(decoder.decode(data, size, buffer, len)){
				cv::Mat image(HEIGHT, WIDTH, CV_8UC3, buffer);

				cv::imshow("image", image);
				if (cv::waitKey(25) == 27)
					break;
			}
		}
	}

	cv::destroyAllWindows();

	capture->stop();

	delete capture;
	delete captureFactory;

	return 0;
}


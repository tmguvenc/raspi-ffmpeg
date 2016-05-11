/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
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
#include "Encoder.h"

#define WIDTH 640
#define HEIGHT 480

tbb::concurrent_bounded_queue<void*> frameQueue;
ICapture *capture;

int main() {
	frameQueue.set_capacity(20);

	capture = new WebcamCapture("/dev/video0");

	capture->init();

	Encoder encoder;
	Decoder decoder;

	if (!encoder.init(AV_CODEC_ID_H264, WIDTH, HEIGHT, 1)) {
		std::cout << "cannot initialize encoder" << std::endl;
		exit(EXIT_FAILURE);
	}

	decoder.setup(AV_CODEC_ID_MJPEG);

	const auto len = HEIGHT * WIDTH * 3;
	char buffer[len] = { 0 };

	capture->start([](void* ptr)
	{
		frameQueue.push(ptr);
	});



	auto ctx = zmq_ctx_new();
	auto socket = zmq_socket(ctx, ZMQ_REP);

	zmq_bind(socket, "tcp://*:5555");

	while (true) {
		void* ptr = nullptr;
		frameQueue.pop(ptr);

		if (ptr != nullptr) {
			VAFrame* frame = static_cast<VAFrame*>(ptr);

			auto decoded = decoder.decode(frame->data(), frame->size(), buffer, len);

			void* encoded_buffer = nullptr;
			size_t encoded_len = 0;

			if (decoded) {
				if (encoder.encode(buffer, len, encoded_buffer, encoded_len)) {
					std::cout << "encoded " << encoded_len << std::endl;

					char buf[10] = { 0 };
					zmq_recv(socket, buf, sizeof buf, 0);
					if (strncmp(buf, "frame", 5) != 0)
						continue;

					zmq_send(socket, encoded_buffer, encoded_len, 0);
				}
			}

			if (cv::waitKey(1) == 27)
				break;

			if (decoded) {
				free(encoded_buffer);
			}

			delete frame;
		}
	}

	cv::destroyAllWindows();

	capture->teardown();
	delete capture;

	return 0;
}

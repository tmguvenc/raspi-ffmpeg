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
#include <opencv2/opencv.hpp>
#include "VAFrame.h"
#include "Encoder.h"
#include <ctime>

#define WIDTH 640
#define HEIGHT 480

tbb::concurrent_bounded_queue<spEncodedBuffer> encodedQueue;
ICapture *capture;
Encoder encoder;

int main() {
	encodedQueue.set_capacity(100);

	capture = new WebcamCapture("/dev/video0");

	capture->init();

	auto ctx = zmq_ctx_new();
	auto socket = zmq_socket(ctx, ZMQ_REP);

	zmq_bind(socket, "tcp://*:5555");

	if (!encoder.init(AV_CODEC_ID_H264, WIDTH, HEIGHT, 1)) {
		std::cout << "cannot initialize encoder" << std::endl;
		exit(EXIT_FAILURE);
	}

	capture->start([](void* ptr)
	{
		auto begin = clock();
		VAFrame* frame = static_cast<VAFrame*>(ptr);
		encodedQueue.push(encoder.encode(frame->data(), frame->size()));
		delete frame;
		//std::cout << "encode time: " << float( clock () - begin )*1000 / CLOCKS_PER_SEC << std::endl;
		std::cout << "push size: " << encodedQueue.size() << std::endl;
	});

	while (true) {
		auto begin = clock();
		spEncodedBuffer encodedBuffer = nullptr;
		encodedQueue.pop(encodedBuffer);

		std::cout << "pop size: " << encodedQueue.size() << std::endl;

		if (encodedBuffer) {
			char buf[10] = { 0 };
			zmq_recv(socket, buf, sizeof buf, 0);
			if (strncmp(buf, "frame", 5) != 0)
				continue;
			zmq_send(socket, encodedBuffer->buffer(), encodedBuffer->len(), 0);
		}
		//std::cout << "send time: " << float(clock() - begin)*1000 / CLOCKS_PER_SEC << std::endl;
	}

	zmq_close(socket);
	zmq_ctx_destroy(ctx);
	cv::destroyAllWindows();

	capture->teardown();
	delete capture;

	return 0;
}

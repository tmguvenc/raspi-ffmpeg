/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <tbb/concurrent_queue.h>
#include "VAFrame.h"
#include "VACapture.h"
#include <zmq.h>
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
}

int main() {
	tbb::concurrent_bounded_queue<VAFrameContainer*> frameQueue;

	frameQueue.set_capacity(100);

	VACapture capture("/dev/video0", &frameQueue);

	auto fmt_ctx = capture.getFormatContext();

	capture.start();

	tbb::tbb_thread *thread = new tbb::tbb_thread(
			[fmt_ctx](tbb::concurrent_bounded_queue<VAFrameContainer*>* queue) {

				auto context = zmq_ctx_new();
				auto socket = zmq_socket(context, ZMQ_REP);
				zmq_bind(socket, "tcp://*:5555");

				char buffer [10];

				while(true) {

					zmq_recv(socket, buffer, sizeof buffer, 0);

					if(strncmp(buffer, "index", 5) != 0) continue;

					VAFrameContainer *frame;

					queue->pop(frame);

					auto f = static_cast<VAFrame*>(frame);

					uint64_t index = f->getFrameIndex();

					zmq_send(socket, &index, sizeof index, 0);

					zmq_recv(socket, buffer, sizeof buffer, 0);

					zmq_send(socket, f->data(), f->size(), 0);

					delete f;
				}

			}, &frameQueue);

	thread->join();
	delete thread;

	capture.teardown();

	return 0;
}

/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "Sender.h"
#include <zmq.h>
#include <iostream>
#include "WebcamCapture.h"
#include <csignal>
#include <memory>

extern "C"
{
#include <libavformat/avformat.h>
}

tbb::concurrent_bounded_queue<VAFrameContainer*> frameQueue;
ICapture *capture;
Sender *sender;

void signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "interrupt signal catched!" << std::endl;

		sender->stop();
		delete sender;

		std::cout << "stopped sender!" << std::endl;

		capture->teardown();
		delete capture;

		std::cout << "stopped capturing!" << std::endl;

		frameQueue.clear();

		std::cout << "cleared queue!" << std::endl;
	}
	exit(EXIT_FAILURE);
}

int main()
{
	auto handler = std::signal(SIGINT, signal_handler);

	if (handler == SIG_ERR)
	{
		std::cerr << "Setup failed\n";
		return EXIT_FAILURE;
	}

	capture = new WebcamCapture("/dev/video0");

	capture->init();
	capture->start([](void* ptr){frameQueue.push(static_cast<VAFrame*>(ptr));});

	sender = new Sender(5555, &frameQueue);
	sender->start();

	capture->teardown();
	delete capture;

	return 0;
}

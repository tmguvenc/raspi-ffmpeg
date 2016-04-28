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

extern "C"
{
#include <libavformat/avformat.h>
}

void signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "interrupt signal catched!" << std::endl;
	}
	else
	{
		std::cout << "asdas asd" << std::endl;
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

	tbb::concurrent_bounded_queue<VAFrameContainer*> frameQueue;
	frameQueue.set_capacity(100);

	ICapture *capture = new WebcamCapture("/dev/video0", &frameQueue);
	capture->init();
	capture->start();

	tbb::tbb_thread *thread = new tbb::tbb_thread(
			[](tbb::concurrent_bounded_queue<VAFrameContainer*>* queue)
			{
				Sender sender(5555, queue);
				sender.start();
			}, &frameQueue);

	thread->join();
	delete thread;

	capture->teardown();
	delete capture;

	return 0;
}

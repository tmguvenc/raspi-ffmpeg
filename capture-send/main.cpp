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
#include <tbb/concurrent_queue.h>

extern "C"
{
#include <libavformat/avformat.h>
}

tbb::concurrent_bounded_queue<VAFrameContainer*> frameQueue;
ICapture *capture;
Sender *sender;

int main()
{
	frameQueue.set_capacity(20);

	capture = new WebcamCapture("/dev/video0");

	capture->init();
	capture->start([](void* ptr)
	{
		frameQueue.push(static_cast<VAFrame*>(ptr));
	});

	sender = new Sender(5555);
	sender->start([]()
	{
		VAFrameContainer* frame;
		frameQueue.pop(frame);
		return frame;
	});

	capture->teardown();
	delete capture;

	return 0;
}

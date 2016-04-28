/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "Sender.h"
#include "VACapture.h"
#include <zmq.h>
#include <iostream>

extern "C"
{
#include <libavformat/avformat.h>
}

int main()
{
	tbb::concurrent_bounded_queue<VAFrameContainer*> frameQueue;

	frameQueue.set_capacity(100);

	VACapture capture("/dev/video0", &frameQueue);

	capture.start();

	tbb::tbb_thread *thread = new tbb::tbb_thread(
			[](tbb::concurrent_bounded_queue<VAFrameContainer*>* queue)
			{
				Sender sender(5555, queue);
				sender.start();
			}, &frameQueue);

	thread->join();
	delete thread;

	capture.teardown();

	return 0;
}

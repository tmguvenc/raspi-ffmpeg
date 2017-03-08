/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <tbb/concurrent_queue.h>
#include <capture.h>
#include <spdlog/spdlog.h>
#include <frame.h>
#include <sender.h>
#include <parser.h>
#include <signal.h>
#include <iostream>

std::atomic<bool> g_run = true;

void handler(int signal) {
	if (signal == SIGINT) {
		std::cout << "QUITTING!!" << std::endl;
		g_run.store(false);
	}
}

void inline clearQueue(tbb::concurrent_bounded_queue<FrameContainer*>* frameQueue) {
	while (!frameQueue->empty()) {
		FrameContainer* frame;
		frameQueue->pop(frame);
		delete frame;
	}
}

int main(int argc, char* argv[]) {
	signal(SIGINT, handler);

	auto logger = spdlog::stdout_color_mt("mainsender");
	Arguments args;

	ArgumentParser parser;

	try {
		args = parser.parse(argc, argv);
	} catch (const std::exception& exception) {
		logger->error("{}", exception.what());
		return -1;
	}
	tbb::concurrent_bounded_queue<FrameContainer*> frameQueue;

	frameQueue.set_capacity(args.balance);

	WebcamCaptureFactory captureFactory;

	logger->info("Starting capture-send");

	CaptureSettings settings(args.width, args.height, 3, args.fps, args.codec);

	while (g_run) {
		auto capture = captureFactory.create(args.url);
		
		capture->init(&settings);

		capture->startAsync([&frameQueue](void* ptr) {
			frameQueue.push(static_cast<Frame*>(ptr));
		});

		auto retry = 0;
		while (!capture->started()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (retry++ == 200){
				logger->error("cannot open video source in {} sec.", 200 * 100 / 1000);
				return -1;
			}
		}

		Sender sender(args.port, settings);

		sender.start([&frameQueue]() {
			FrameContainer* frame;
			frameQueue.pop(frame);
			return frame;
		});

		clearQueue(&frameQueue);

		logger->info("Restarting capture-send");
		assert(frameQueue.empty() && "frame queue is not empty!");
	}

	clearQueue(&frameQueue);
	spdlog::drop("mainsender");

	return 0;
}

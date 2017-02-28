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
#include <sstream>
#include <parser.h>

tbb::concurrent_bounded_queue<FrameContainer*> frameQueue;
std::shared_ptr<spdlog::logger> logger;

int main(int argc, char* argv[])
{
	logger = spdlog::stdout_color_mt("mainsender");
	Arguments args;

	ArgumentParser parser;

	try {
		args = parser.parse(argc, argv);
	} catch (std::exception& exception) {
		logger->error("{}", exception.what());
		return -1;
	}

	frameQueue.set_capacity(args.balance);

	WebcamCaptureFactory captureFactory;

	logger->info("Starting capture-send");

	CaptureSettings settings(args.width, args.height, 3, args.fps, args.codec);

	while (true)
	{
		auto capture = captureFactory.create(args.url, spdlog::stdout_color_mt("capturesender"));
		
		capture->init(&settings);

		capture->startAsync([](void* ptr)
		{
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

		auto sender = new Sender(args.port, settings);
		sender->start([]()
		{
			FrameContainer* frame;
			frameQueue.pop(frame);
			return frame;
		});

		while (!frameQueue.empty()) {			
			FrameContainer* frame;
			frameQueue.pop(frame);
			delete frame;
		}

		spdlog::drop("capturesender");
		capture->stop();
		delete capture;
		delete sender;

		logger->info("Restarting capture-send");
		frameQueue.clear();
	}

	return 0;
}
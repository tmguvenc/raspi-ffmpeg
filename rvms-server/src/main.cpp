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
#include <videoframesender.h>
#include <parser.h>
#include <common_utils.h>

#ifdef __linux__
#include <wiringPi.h>
#endif

int main(int argc, char* argv[]) {

	auto logger = spdlog::stdout_color_mt("mainsender");
	Arguments args;

	ArgumentParser parser;

	try {
		args = parser.parse(argc, argv);
	} catch (const std::exception& exception) {
		logger->error("{}", exception.what());
		return -1;
	}

#ifdef __linux__
	wiringPiSetup();
#endif

	WebcamCaptureFactory captureFactory;

	logger->info<std::string>("Starting capture-send");

	CaptureSettings settings(args.width, args.height, 3, args.fps, args.codec);

	while (true) {
		try {
			tbb::concurrent_bounded_queue<FrameContainer*> frameQueue;
			frameQueue.set_capacity(args.balance);

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

			VideoFrameSender sender(args.port, args.width, args.height, args.codec);
			sender.start([&frameQueue]() {
				FrameContainer* frame;
				frameQueue.pop(frame);
				assert(frame != nullptr && "frame is null");
				return frame;
			});

			clearQueue(&frameQueue);

			logger->info<std::string>("Restarting capture-send");
			assert(frameQueue.empty() && "frame queue is not empty!");
		} catch (const std::exception& ex) {
			logger->error("{}", ex.what());
			return -1;
		}
	}

	spdlog::drop("mainsender");

	return 0;
}

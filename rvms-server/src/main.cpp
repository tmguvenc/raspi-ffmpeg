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
#include <jobdistributor.h>
#include <parser.h>
#include <common_utils.h>
#include <rgpio.h>

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

	GPIO::setup();

	WebcamCaptureFactory captureFactory;

	logger->info<std::string>("Starting {0}", argv[0]);

	CaptureSettings settings(args.width, args.height, 3, args.fps, args.codec);

	while (true) {
		try {
			JobDistributor distributor(args.port, args.width, args.height, args.codec, args.fps, 1000, 8, { 0, 2, 3, 4 }, { 0, 2, 3, 4 });
			distributor.start();
		} catch (const std::exception& ex) {
			logger->error("{}", ex.what());
			return -1;
		}
	}

	spdlog::drop("mainsender");

	return 0;
}

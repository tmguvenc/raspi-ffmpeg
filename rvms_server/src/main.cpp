/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <spdlog/spdlog.h>
#include <jobdistributor.h>
#include <parser.h>
#include <rgpio.h>

int main(int argc, char* argv[]) {

	auto logger = spdlog::stdout_color_mt("mainsender");
	ApplicationParams args;

	if (argc != 2) {
		logger->error("usage: {} configFileName", argv[0]);
		return -1;
	}

	try {
		args = ArgumentParser().parse(argv[1]);
	} catch (const std::exception& exception) {
		logger->error("{}", exception.what());
		return -1;
	}

	GPIO::setup();

	logger->info<std::string>("Starting {0}", argv[0]);

	while (true) {
		try {
			JobDistributor distributor(args);
			distributor.start();
			logger->info<std::string>("Restarting {0}", argv[0]);
		} catch (const std::exception& ex) {
			logger->error("{}", ex.what());
			return -1;
		}
	}

	spdlog::drop("mainsender");

	return 0;
}

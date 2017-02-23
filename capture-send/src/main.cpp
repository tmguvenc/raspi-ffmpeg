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
#include <unordered_map>
#include <sstream>

tbb::concurrent_bounded_queue<FrameContainer*> frameQueue;
std::shared_ptr<spdlog::logger> logger;

//default options
std::unordered_map<std::string, std::string> options = {
	{ "-p", "5555" },
	{ "-b", "20" },
	{ "-u", "/dev/video0" },
	{ "-r", "640x480" },
	{ "-c", "mjpeg" },
	{ "-f", "15" }
};

CaptureSettings parse_arguments(int argc, char* argv[]);

inline static bool is_number(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (std::getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

bool checkParams()
{
	if (!is_number(options["-b"])) {
		logger->error("buffer size is not valid");
		return false;
	}
	if (!is_number(options["-p"])) {
		logger->error("port number is not valid");
		return false;
	}
	if (!is_number(options["-f"])) {
		logger->error("FPS value is not valid");
		return false;
	}

	auto codec_name = options["-c"];

	if (codec_name != "h264" && codec_name != "mjpeg" && codec_name != "raw") {
		logger->error("codec name is not valid. (h264, mjpeg or raw)");
		return false;
	}

	auto res = split(options["-r"], 'x');

	if (res.size() != 2 || !is_number(res[0]) || !is_number(res[1])) {
		logger->error("resolution string is not valid. (640x480, 1024x768, etc.)");
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	logger = spdlog::stdout_color_mt("mainsender");

	auto settings = parse_arguments(argc, argv);

	if (!checkParams()) return -1;

	frameQueue.set_capacity(std::atoi(options["-b"].c_str()));

	WebcamCaptureFactory captureFactory;

	logger->info("Starting capture-send");

	while (true)
	{
		auto capture = captureFactory.create(options["-u"], spdlog::stdout_color_mt("capturesender"));

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

		auto sender = new Sender(std::atoi(options["-p"].c_str()), settings);
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

CaptureSettings parse_arguments(int argc, char* argv[])
{
	for (auto i = 1; i < argc; ++i)
	{
		auto op = options.find(argv[i]);
		if (op != options.end())
			op->second = argv[i + 1];
	}

	uint32_t w = 640, h = 480, f = 15;

	AVCodecID codec;

	auto res = split(options["-r"], 'x');
	if (res.size() == 2 && is_number(res[0]) && is_number(res[1])) {
		w = atoi(res[0].c_str());
		h = atoi(res[1].c_str());
		logger->info("width: {}", w);
		logger->info("height: {}", h);
	}
	auto codec_name = options["-c"];

	if (codec_name == "mjpeg")
		codec = AV_CODEC_ID_MJPEG;
	else if (codec_name == "h264")
		codec = AV_CODEC_ID_H264;
	else if (codec_name == "raw")
		codec = AV_CODEC_ID_RAWVIDEO;
	else
		codec = AV_CODEC_ID_MJPEG;

	if (is_number(options["-f"])) {
		f = atoi(options["-f"].c_str());
	}

	return CaptureSettings(w, h, 3, f, codec);
}

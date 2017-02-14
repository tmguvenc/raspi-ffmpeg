/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#include <tbb/concurrent_queue.h>
#include <capture.h>
#include <spdlog/spdlog.h>
#include <frame.h>
#include <sender.h>
#include <unordered_map>
#include <iostream>
#include <sstream>

extern "C"
{
#include <libavformat/avformat.h>
}

tbb::concurrent_bounded_queue<FrameContainer*> frameQueue;

//default options
std::unordered_map<std::string, std::string> options = {
	{ "-p", "5555" },
	{ "-b", "20" },
	{ "-u", "dev/video0" },
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
		std::cerr <<  "buffer size is not valid" << std::endl;
		return false;
	}
	if (!is_number(options["-p"])) {
		std::cerr <<  "port number is not valid" << std::endl;
		return false;
	}
	if (!is_number(options["-f"])) {
		std::cerr <<  "FPS value is not valid" << std::endl;
		return false;
	}

	auto codec_name = options["-c"];

	if (!(codec_name == "h264" || codec_name == "mpjeg" || codec_name == "raw")) {
		std::cerr << "codec name is not valid. (h264, mjpeg or raw)" << std::endl;
		return false;
	}

	auto res = split(options["-r"], 'x');

	if (res.size() != 2 || !is_number(res[0]) || !is_number(res[1])) {
		std::cerr << "resolution string is not valid. (640x480, 1024x768, etc.)" << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	auto settings = parse_arguments(argc, argv);

	if (!checkParams()) return -1;

	frameQueue.set_capacity(std::atoi(options["-b"].c_str()));

	WebcamCaptureFactory captureFactory;
	auto capture = captureFactory.create(options["-u"], spdlog::stdout_color_mt("capturesender"));

	capture->init(&settings);

	capture->start([](void* ptr)
	{
		frameQueue.push(static_cast<Frame*>(ptr));
	});

	auto sender = new Sender(std::atoi(options["-p"].c_str()));
	sender->start([]()
	{
		FrameContainer* frame;
		frameQueue.pop(frame);
		return frame;
	});

	capture->stop();
	delete capture;

	delete sender;

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

	return CaptureSettings(w, h, 3, f, codec);
}
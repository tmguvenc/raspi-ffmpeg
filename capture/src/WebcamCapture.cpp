/*
 * VACapture.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "WebcamCapture.h"

#include "VAFrame.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include "CaptureSettings.h"
#include <chrono>
#include <thread>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

inline static bool is_number(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

WebcamCapture::WebcamCapture(const std::string& connectionString) :
		m_connectionString(connectionString), m_width(0), m_height(0), m_channels(0), m_completed(false), m_run(false), m_indexofVideoStream(
				0), m_formatContext(nullptr), m_options(nullptr), m_codecContext(nullptr), m_settings(nullptr) {

}

WebcamCapture::~WebcamCapture() {
	std::cout << "closing..." << std::endl;
}

void WebcamCapture::init(CaptureSettings* settings) {
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
	m_settings = settings;
	assert(m_formatContext == nullptr && "format context is not null.");

	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return reinterpret_cast<int>(ctx);};
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);

	if (settings != nullptr) {

		auto codec = avcodec_get_name((AVCodecID) settings->getCodecId());
		if (codec != nullptr) {
			av_dict_set(&m_options, "input_format", codec, 0);
			std::cout << "codec: " << codec << std::endl;
		} else {
			av_dict_set(&m_options, "input_format", "mjpeg", 0);
			std::cout << "codec not found. setting it to mjpeg" << std::endl;
		}

		auto fps = std::to_string(settings->getFPS());

		if (is_number(fps)) {
			av_dict_set(&m_options, "framerate", fps.c_str(), 0);
			std::cout << "frame rate: " << fps.c_str() << std::endl;
		} else {
			av_dict_set(&m_options, "framerate", "15", 0);
			std::cout << "wrong frame rate format. setting it to 15" << std::endl;
		}

		if (is_number(std::to_string(settings->getWidth())) && is_number(std::to_string(settings->getHeight()))) {
			m_width = settings->getWidth();
			m_height = settings->getHeight();
			std::stringstream ss;
			ss << m_width << "x" << m_height;
			auto vs = ss.str().c_str();
			av_dict_set(&m_options, "video_size", vs, 0);
			std::cout << "video size: " << vs << std::endl;
		} else {
			av_dict_set(&m_options, "video_size", "640x480", 0);
			std::cout << "wrong resolution format. setting it to 640x480" << std::endl;
		}
	} else {
		av_dict_set(&m_options, "framerate", "15", 0);
		av_dict_set(&m_options, "input_format", "mjpeg", 0);
		av_dict_set(&m_options, "video_size", "640x480", 0);

		std::cout << "codec: " << "mjpeg" << std::endl;
		std::cout << "frame rate: " << "15" << std::endl;
		std::cout << "video size: " << "640x480" << std::endl;

		m_width = 640;
		m_height = 480;
	}
}

VAFrameContainer* WebcamCapture::grabFrame() {
	while (true) {
		auto frame = new VAFrame(m_width, m_height, 3, 0);
		auto pkt = frame->getPacket();
		auto result = av_read_frame(m_formatContext, pkt);
		if (result < 0) {
			delete frame;
			return nullptr;
		}
		if (pkt->stream_index == m_indexofVideoStream)
			return frame;
		delete frame;
	}
}

void WebcamCapture::start(CaptureCallback func) {

	auto input_format = av_find_input_format("video4linux2");
	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), input_format, &m_options) < 0) {
		std::cout << "cannot open input " << m_connectionString << std::endl;
		return;
	}

	std::cout << "started grabbing" << std::endl;

	m_run = true;

	while (m_run) {
		auto frame = grabFrame();
		if (frame)
			func(frame);
		else
			break;
	}

	m_completed = true;
	func(nullptr);
	std::cout << "completed" << std::endl;
}

void WebcamCapture::startAsync(CaptureCallback func) {

	m_captureHandle = std::async(std::launch::async, [this](CaptureCallback f) {start(f);}, func);
}

void WebcamCapture::stop() {

	// in case of the user stops capturing too soon.
	while (!m_run) {
		std::cout << "capturing is not started yet" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	m_run = false;

	m_formatContext->interrupt_callback.opaque = reinterpret_cast<void*>(1);

	// wait for capturing finish
	m_captureHandle.get();

	av_dict_free(&m_options);
	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	m_options = nullptr;
	m_formatContext = nullptr;
}

bool WebcamCapture::completed() {
	return m_completed;
}

void* WebcamCapture::getCodecInfo() {
	if (m_codecContext != nullptr)
		return m_codecContext;

	auto input_format = av_find_input_format("video4linux2");
	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), input_format, &m_options) < 0) {
		std::cout << "cannot open input " << m_connectionString << std::endl;
		return nullptr;
	}

	avformat_find_stream_info(m_formatContext, nullptr);

	m_indexofVideoStream = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	m_codecContext = m_formatContext->streams[m_indexofVideoStream]->codec;

	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	m_options = nullptr;
	m_formatContext = nullptr;

	// re-initialize
	init(m_settings);

	return m_codecContext;
}

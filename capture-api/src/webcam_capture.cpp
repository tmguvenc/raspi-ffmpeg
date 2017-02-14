/*
 * VACapture.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <webcam_capture.h>

#include <frame.h>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <capture_settings.h>
#include <chrono>
#include <thread>
#include <capture_utils.h>
#include <spdlog/spdlog.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

WebcamCapture::WebcamCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger> logger) :
m_connectionString(connectionString), m_width(0), m_height(0),
m_channels(0), m_completed(false),
m_run(false),
m_indexofVideoStream(0),
m_formatContext(nullptr),
m_options(nullptr),
m_codecContext(nullptr),
m_logger(std::move(logger))
{
	m_settings = new CaptureSettings;
	spdlog::set_async_mode(4096);
}

WebcamCapture::~WebcamCapture() {
	if (m_settings)
	{
		delete m_settings;
		m_settings = nullptr;
	}
	m_logger->info("closing {}", m_connectionString);
}

void WebcamCapture::init(CaptureSettings* settings) {
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
	if (settings != nullptr) {
		memcpy(m_settings, settings, sizeof(CaptureSettings));
	}

	assert(m_formatContext == nullptr && "format context is not null.");

	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return static_cast<int>(reinterpret_cast<intptr_t>(ctx)); };
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);

	auto codec_name = avcodec_get_name(static_cast<AVCodecID>(m_settings->getCodecId()));
	if (codec_name != nullptr) {
		assert(av_dict_set(&m_options, "input_format", codec_name, 0) >= 0);
		m_logger->info("codec: {}", codec_name);
	}
	else {
		assert(av_dict_set(&m_options, "input_format", "mjpeg", 0) >= 0);
		m_logger->warn("codec not found. setting it to {}", "mjpeg");
	}

	auto fps = std::to_string(m_settings->getFPS());

	if (is_number(fps)) {
		assert(av_dict_set(&m_options, "framerate", fps.c_str(), 0) >= 0);
		m_logger->info("frame rate: {}", m_settings->getFPS());
	}
	else {
		assert(av_dict_set(&m_options, "framerate", "15", 0) >= 0);
		m_logger->warn("wrong frame rate format. setting it to {}", 15);
	}

	if (is_number(std::to_string(m_settings->getWidth())) && is_number(std::to_string(m_settings->getHeight()))) {
		m_width = m_settings->getWidth();
		m_height = m_settings->getHeight();
		std::stringstream ss;
		ss << m_width << "x" << m_height;
		auto vs = ss.str();
		assert(av_dict_set(&m_options, "video_size", vs.c_str(), 0) >= 0);
		m_logger->info("video size: {}", vs);
	}
	else {
		assert(av_dict_set(&m_options, "video_size", "640x480", 0) >= 0);
		m_logger->warn("wrong resolution format. setting it to {}", "640x480");
	}
}

FrameContainer* WebcamCapture::grabFrame() {
	while (true) {
		auto frame = new Frame(m_width, m_height, 3, 0);
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

	auto input_format = find_input_format();

	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), input_format, &m_options) < 0) {
		m_logger->error("cannot open input {}", m_connectionString);
		return;
	}

	m_logger->info("started grabbing");

	m_run.store(true);

	while (m_run) {
		auto frame = grabFrame();
		if (frame)
			if (func)
				func(frame);
			else
				delete frame;
		else
			break;
	}

	m_completed = true;
	func(nullptr);
	m_logger->info("completed");
}

void WebcamCapture::startAsync(CaptureCallback func) {

	auto future = std::async(std::launch::async, [this](CaptureCallback f) {start(f); }, func);

	m_captureHandle = std::move(future);
}

void WebcamCapture::stop() {

	// in case of the user stops capturing too soon.
	while (!m_run) {
		m_logger->warn("capturing is not started yet");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	m_run.store(false);

	m_formatContext->interrupt_callback.opaque = reinterpret_cast<void*>(1);

	// wait for capturing finish
	m_captureHandle.get();

	av_dict_free(&m_options);
	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	m_options = nullptr;
	m_formatContext = nullptr;
	m_logger->info("capturing stopped");
}

bool WebcamCapture::completed() {
	return m_completed;
}

void* WebcamCapture::getCodecInfo() {
	if (m_codecContext != nullptr)
		return m_codecContext;

	auto input_format = find_input_format();

	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), input_format, &m_options) < 0) {
		m_logger->error("cannot open input {}", m_connectionString);
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

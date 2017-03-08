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

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

WebcamCapture::WebcamCapture(const std::string& connectionString) :
m_connectionString(connectionString), m_width(0), m_height(0),
m_channels(0), m_completed(false),
m_run(false),
m_indexofVideoStream(0),
m_formatContext(nullptr),
m_options(nullptr),
m_codecContext(nullptr){
	m_settings = new CaptureSettings;
}

WebcamCapture::~WebcamCapture() {
	if (m_settings) {
		delete m_settings;
		m_settings = nullptr;
	}
}

void WebcamCapture::init(CaptureSettings* settings) {
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
	if (settings != nullptr) {
		memcpy(m_settings, settings, sizeof(CaptureSettings));
	}

	if (m_formatContext != nullptr) {
		throw std::invalid_argument("Format Context must be null before initialization.");
	}

	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return static_cast<int>(reinterpret_cast<intptr_t>(ctx)); };
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);

	auto codec_name = avcodec_get_name(static_cast<AVCodecID>(m_settings->getCodecId()));
	if (codec_name != nullptr) {
		av_dict_set(&m_options, "input_format", codec_name, 0);
	}
	else {
		av_dict_set(&m_options, "input_format", "mjpeg", 0);
	}

	auto fps = std::to_string(m_settings->getFPS());

	if (is_number(fps)) {
		av_dict_set(&m_options, "framerate", fps.c_str(), 0);
	}
	else {
		av_dict_set(&m_options, "framerate", "15", 0);
	}

	if (is_number(std::to_string(m_settings->getWidth())) && is_number(std::to_string(m_settings->getHeight()))) {
		m_width = m_settings->getWidth();
		m_height = m_settings->getHeight();
		std::stringstream ss;
		ss << m_width << "x" << m_height;
		auto vs = ss.str();
		av_dict_set(&m_options, "video_size", vs.c_str(), 0);
	}
	else {
		av_dict_set(&m_options, "video_size", "640x480", 0);
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
		throw std::invalid_argument("cannot open [ " + m_connectionString + " ]");
	}

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
}

void WebcamCapture::startAsync(CaptureCallback func) {

	auto future = std::async(std::launch::async, [this](CaptureCallback f) {start(f); }, func);

	m_captureHandle = std::move(future);
}

void WebcamCapture::stop() {

	// in case of the user stops capturing too soon.
	while (!m_run) {
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
}

bool WebcamCapture::started()
{
	return m_run;
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
		throw std::invalid_argument("cannot open [ " + m_connectionString + " ]");
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

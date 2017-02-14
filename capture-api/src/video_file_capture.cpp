/*
 * VideoFileCapture.cpp
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat G�ven�
 */

#include <assert.h>
#include <iostream>
#include <video_file_capture.h>
#include <frame.h>
#include <spdlog/spdlog.h>
#include <capture_utils.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

VideoFileCapture::VideoFileCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger> logger) :
m_connectionString(connectionString), m_width(0), m_height(0), m_channels(0), 
m_completed(false), 
m_run(false), 
m_indexofVideoStream(0), 
m_formatContext(nullptr), 
m_codecContext(nullptr),
m_logger(std::move(logger))
{

}

VideoFileCapture::~VideoFileCapture() {
}

void VideoFileCapture::init(CaptureSettings* /*settings*/) {
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();

	assert(m_formatContext == nullptr && "format context is not null.");

	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return static_cast<int>(reinterpret_cast<intptr_t>(ctx)); };
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);
}

FrameContainer* VideoFileCapture::grabFrame() {
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

void VideoFileCapture::start(CaptureCallback func) {

	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), nullptr, nullptr) < 0) {
		m_logger->error("cannot open input {}", m_connectionString);
		return;
	}

	m_logger->info("started grabbing");

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
	m_logger->info("completed");
}

void VideoFileCapture::startAsync(CaptureCallback func) {

	auto future = std::async(std::launch::async, [this](CaptureCallback f) {start(f); }, func);

	m_captureHandle = std::move(future);
}

void VideoFileCapture::stop() {

	// in case of the user stops capturing too soon.
	while (!m_run) {
		m_logger->warn("capturing is not started yet");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	m_run = false;

	m_formatContext->interrupt_callback.opaque = reinterpret_cast<void*>(1);

	// wait for capturing finish
	m_captureHandle.get();

	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	avcodec_close(m_codecContext);
	m_formatContext = nullptr;
	m_codecContext = nullptr;
}

bool VideoFileCapture::completed() {
	return m_completed;
}

bool VideoFileCapture::started()
{
	return m_run;
}

void* VideoFileCapture::getCodecInfo() {
	if (m_codecContext != nullptr)
		return m_codecContext;

	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(), nullptr, nullptr) < 0) {
		m_logger->error("cannot open input {}", m_connectionString);
		return nullptr;
	}

	avformat_find_stream_info(m_formatContext, nullptr);

	m_indexofVideoStream = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	m_codecContext = m_formatContext->streams[m_indexofVideoStream]->codec;

	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	m_formatContext = nullptr;

	return m_codecContext;
}

/*
 * VACapture.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <audio_capture.h>

#include <frame.h>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <capture_settings.h>
#include <chrono>
#include <thread>
#include <capture_utils.h>
#include <audio_frame.h>
#include <cstring>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

AudioCapture::AudioCapture(const std::string& connectionString) :
m_connectionString(connectionString),
m_completed(false),
m_run(false),
m_indexofVideoStream(0),
m_formatContext(nullptr),
m_options(nullptr),
m_codecContext(nullptr){
	m_settings = new CaptureSettings;
}

AudioCapture::~AudioCapture() {
	if (m_settings) {
		delete m_settings;
		m_settings = nullptr;
	}
}

void AudioCapture::init(CaptureSettings* settings) {
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

	av_dict_set(&m_options, "channels", "1", 0);
	av_dict_set(&m_options, "sample_rate", "44100", 0);
}

FrameContainer* AudioCapture::grabFrame() {
	while (true) {
		auto frame = new AudioFrame(0);
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

void AudioCapture::start(CaptureCallback func) {

	auto input_format = find_input_format(false);

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

void AudioCapture::startAsync(CaptureCallback func) {

	auto future = std::async(std::launch::async, [this](CaptureCallback f) {start(f); }, func);

	m_captureHandle = std::move(future);
}

void AudioCapture::stop() {

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

bool AudioCapture::started()
{
	return m_run;
}

bool AudioCapture::completed() {
	return m_completed;
}

void* AudioCapture::getCodecInfo() {
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

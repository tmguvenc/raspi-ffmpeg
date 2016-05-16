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
#include <string.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

WebcamCapture::WebcamCapture(const std::string& connectionString) :
		m_connectionString(connectionString), m_width(0), m_height(0), m_channels(
				0), m_completed(false), m_run(false), m_indexofVideoStream(0), m_thread(
				nullptr), m_formatContext(nullptr), m_options(nullptr)
{

}

WebcamCapture::~WebcamCapture()
{
	std::cout << "closing..." << std::endl;
}

void WebcamCapture::init()
{
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();

	assert(m_formatContext == nullptr && "format context is not null.");

	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return reinterpret_cast<int>(ctx);};
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);


	av_dict_set(&m_options, "framerate", "15", 0);
	//av_dict_set(&m_options, "input_format", "mjpeg", 0);
	av_dict_set(&m_options, "video_size", "640x480", 0);

	auto input_format = av_find_input_format("video4linux2");
	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, m_connectionString.c_str(),
			input_format, &m_options) < 0)
	{
		std::cerr << "cannot open input " << m_connectionString << std::endl;
		exit(1);
	}

	avformat_find_stream_info(m_formatContext, nullptr);

	m_indexofVideoStream = av_find_best_stream(m_formatContext,
			AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

	auto stream = m_formatContext->streams[m_indexofVideoStream];

	m_width = stream->codec->width;
	m_height = stream->codec->height;

#ifndef NDEBUG

	std::cout << "*****Codec: " << avcodec_get_name(stream->codec->codec_id) << std::endl;
	std::cout << "*****Resolution: " << m_width << "x" << m_height << std::endl;

	assert(m_width && "width cannot be 0");
	assert(m_height && "height cannot be 0");
#endif
}

void WebcamCapture::teardown()
{
	stop();
}

VAFrameContainer* WebcamCapture::grabFrame()
{
	while (true)
	{
		auto frame = new VAFrame(m_width, m_height, 3, 0);
		auto pkt = frame->getPacket();
		auto result = av_read_frame(m_formatContext, pkt);
		if (result < 0)
		{
 			delete frame;
			return nullptr;
		}
		if (pkt->stream_index == m_indexofVideoStream) return frame;
		delete frame;
	}
}

void WebcamCapture::start(CaptureCallback func)
{
	m_thread = new tbb::tbb_thread([this, func]()
	{
		std::cout << "started grabbing" << std::endl;

		m_run = true;

		while (m_run)
		{
			auto frame = grabFrame();
			if (frame) func(frame);
			else break;
		}
		m_completed = true;
		func(nullptr);
		std::cout << "completed" << std::endl;
	});
}

void WebcamCapture::stop()
{
	m_run = false;

	m_formatContext->interrupt_callback.opaque = reinterpret_cast<void*>(1);

	if (m_thread)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}

	av_dict_free(&m_options);
	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
	m_options = nullptr;
	m_formatContext = nullptr;
}

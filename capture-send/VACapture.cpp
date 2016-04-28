/*
 * VACapture.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "VACapture.h"
#include "VAFrame.h"
#include <iostream>
#include <assert.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

VACapture::VACapture(const std::string& connectionString,
		tbb::concurrent_bounded_queue<VAFrameContainer*>* queue) :
		m_width(0), m_height(0), m_channels(0), m_totalFrameCount(0), m_frameIndex(
				0), m_completed(false), m_thread(nullptr), m_queue(queue)
{
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();
	AVDictionary *options = NULL;
	m_formatContext = avformat_alloc_context();

	m_formatContext->interrupt_callback.callback = [](void* ctx)
	{	return reinterpret_cast<int>(ctx);};
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(nullptr);

	auto input_format = av_find_input_format("video4linux2");

	assert(input_format && "cannot found input format");

	// framerate needs to set before opening the v4l2 device
	av_dict_set(&options, "framerate", "15", 0);
	// This will not work if the camera does not support h264. In that case
	// remove this line. I wrote this for Raspberry Pi where the camera driver
	// can stream h264.
	av_dict_set(&options, "input_format", "mjpeg", 0);
	av_dict_set(&options, "video_size", "640x480", 0);

	// open input file, and allocate format context
	if (avformat_open_input(&m_formatContext, connectionString.c_str(),
			input_format, &options) < 0)
	{
		std::cerr << "cannot open input " << connectionString << std::endl;
		exit(1);
	}

	avformat_find_stream_info(m_formatContext, nullptr);

	m_indexofVideoStream = av_find_best_stream(m_formatContext,
			AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

	std::cout << "stream index = " << m_indexofVideoStream << std::endl;
	std::cout << "stream count = " << m_formatContext->nb_streams << std::endl;

	auto stream = m_formatContext->streams[m_indexofVideoStream];

	m_framePeriod = static_cast<double>(stream->r_frame_rate.den)
			/ static_cast<double>(stream->r_frame_rate.num);
	auto timeBase = stream->time_base;
	m_timeBaseMultiplier = static_cast<double>(timeBase.num)
			/ static_cast<double>(timeBase.den);

	m_totalFrameCount = stream->nb_frames;

	m_width = stream->codec->width;
	m_height = stream->codec->height;

	std::cout << "*****Codec: " << stream->codec->codec_id << std::endl;
	std::cout << "*****Resolution: " << m_width << "x" << m_height << std::endl;

	assert(m_width && "width cannot be 0");
	assert(m_height && "height cannot be 0");
}

VACapture::~VACapture()
{
}

void VACapture::teardown()
{
	m_formatContext->interrupt_callback.opaque = reinterpret_cast<void*>(1);

	if (m_thread)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}

	avformat_close_input(&m_formatContext);
	avformat_free_context(m_formatContext);
}

VAFrameContainer* VACapture::grabFrame()
{
	while (true)
	{
		auto frame = new VAFrame(m_width, m_height, 3, m_totalFrameCount);
		auto pkt = frame->getPacket();
		auto result = av_read_frame(m_formatContext, pkt);
		if (result < 0)
		{
			delete frame;
			return nullptr;
		}
		if (pkt->stream_index == m_indexofVideoStream)
		{
			frame->m_index = ++m_frameIndex;
			return frame;
		}
		delete frame;
	}
}

void VACapture::start()
{
	std::cout << "started grabbing" << std::endl;

	m_thread = new tbb::tbb_thread([this]()
	{
		while (true)
		{
			auto frame = grabFrame();
			if (frame)
			{
				m_queue->push(frame);
				std::cout << "pushed frame " << m_frameIndex << std::endl;
			}
			else break;
		}
		m_completed = true;
		m_queue->push(nullptr);
		std::cout << "completed" << std::endl;
	});
}

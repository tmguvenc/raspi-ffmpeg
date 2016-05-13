/*
 * Decoder.cpp
 *
 *  Created on: 2 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "Decoder.h"
#include <assert.h>
#include <iostream>
#include "ICapture.h"
#include <malloc.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
}

Decoder::Decoder(int destWidth, int destHeight) :
		m_codecContext(nullptr), m_codec(nullptr), m_swsContext(nullptr), m_frame(nullptr), m_scaledFrame(nullptr), m_destWidth(
				destWidth), m_destHeight(destHeight) {

}

Decoder::~Decoder() {

}

bool Decoder::setup(AVCodecID codecId, AVPixelFormat pix_fmt) {
	assert(!m_codecContext);
	assert(!m_codec);
	assert(!m_swsContext);
	assert(!m_frame);
	assert(!m_scaledFrame);

	m_codec = avcodec_find_decoder(codecId);

	if (!m_codec) {
		std::cout << "Cannot find codec" << std::endl;
		return false;
	}

	m_codecContext = avcodec_alloc_context3(m_codec);
	m_codecContext->pix_fmt = pix_fmt;

	if (m_codec->capabilities & CODEC_CAP_TRUNCATED)
		m_codecContext->flags |= CODEC_FLAG_TRUNCATED;

	if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0) {
		std::cout << "Cannot open context" << std::endl;
		return false;
	}

	m_frame = av_frame_alloc();

	return setupScale();
}

bool Decoder::setup(ICapture* capture) {
	assert(capture);
	assert(!m_codecContext);
	assert(!m_codec);
	assert(!m_swsContext);
	assert(!m_frame);
	assert(!m_scaledFrame);

	auto temp_codec = static_cast<AVCodecContext*>(capture->getCodecInfo());

	std::cout << "codec: " << avcodec_get_name(temp_codec->codec_id) << std::endl;
	m_codec = avcodec_find_decoder(temp_codec->codec_id);
	if (!m_codec) {
		std::cout << "Cannot find codec";
		return false;
	}

	auto pix = temp_codec->pix_fmt;
	auto width = temp_codec->width;
	auto height = temp_codec->height;

	m_codecContext = avcodec_alloc_context3(m_codec);
	m_codecContext->pix_fmt = pix;
	m_codecContext->width = width;
	m_codecContext->height = height;

	if (m_codec->capabilities & CODEC_CAP_TRUNCATED)
		m_codecContext->flags |= CODEC_FLAG_TRUNCATED;

	if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0) {
		std::cout << "Cannot open context";
		return false;
	}

	m_frame = av_frame_alloc();

	return setupScale();
}

bool Decoder::setupScale() {
	m_swsContext = sws_getCachedContext(m_swsContext, width(), height(), m_codecContext->pix_fmt, m_destWidth, m_destHeight,
			AV_PIX_FMT_BGR24, SWS_BICUBIC, nullptr, nullptr, nullptr);

	if (!m_swsContext) {
		std::cout << "Cannot create context" << std::endl;
		return false;
	}

	m_scaledFrame = av_frame_alloc();

	if (av_image_alloc(m_scaledFrame->data, m_scaledFrame->linesize, m_destWidth, m_destHeight, AV_PIX_FMT_BGR24, 1) < 0) {
		std::cout << "Cannot allocate image" << std::endl;
		return false;
	}

	return true;
}

int Decoder::width() {
	return m_frame->width ? m_frame->width : m_codecContext->width;
}

int Decoder::height() {
	return m_frame->height ? m_frame->height : m_codecContext->height;
}

void Decoder::teardown() {
	assert(m_codecContext);
	assert(m_codec);
	assert(m_swsContext);
	assert(m_frame);
	assert(m_scaledFrame);

	av_freep(m_frame->data[0]);
	av_frame_free(&m_frame);

	av_freep(&m_scaledFrame->data[0]);
	av_frame_free(&m_scaledFrame);

	avcodec_close(m_codecContext);
	avcodec_free_context(&m_codecContext);
	sws_freeContext(m_swsContext);

	m_codecContext = nullptr;
	m_codec = nullptr;
	m_swsContext = nullptr;
	m_frame = nullptr;
	m_scaledFrame = nullptr;
}

bool Decoder::decode(void* inputData, size_t inputLen, void* data, size_t len) {
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = static_cast<uint8_t*>(inputData);
	packet.size = static_cast<int>(inputLen);
	auto result = decode(&packet, data, len);
	av_packet_unref(&packet);
	return result;
}

bool Decoder::decode(AVPacket* packet, void* data, size_t len) {
	assert(m_swsContext != nullptr);

	int gotFrame;
	if (avcodec_decode_video2(m_codecContext, m_frame, &gotFrame, packet) >= 0) {
		if (!data)
			return false;
		if (gotFrame) {
			if (sws_scale(m_swsContext, m_frame->data, m_frame->linesize, 0, m_frame->height, m_scaledFrame->data,
					m_scaledFrame->linesize) < 0) {
				std::cout << "cannot scale in decode" << std::endl;
				return false;
			} else {
				memcpy(data, m_scaledFrame->data[0],
						MIN(len, static_cast<size_t>(m_codecContext->width * m_codecContext->height * 3)));
			}

			return true;
		}
	} else {
		std::cout << packet->stream_index << " ERROR" << std::endl;
	}
	return false;
}

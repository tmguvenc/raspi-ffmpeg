/*
 * AudioDecoder.cpp
 *
 *  Created on: 2 May 2016
 *      Author: Turan Murat Güvenç
 */

#include <audio_decoder.h>
#include <assert.h>
#include <iostream>
#include <capture_utils.h>
#include <stdexcept>


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

AudioDecoder::AudioDecoder() :
m_codecContext(nullptr), 
m_codec(nullptr), 
m_frame(nullptr)
{

}

AudioDecoder::~AudioDecoder() {

}

bool AudioDecoder::setup(AVCodecID codecId) {
	assert(!m_codecContext);
	assert(!m_codec);
	assert(!m_frame);

	m_codec = avcodec_find_decoder(codecId);

	if (!m_codec) {
		throw std::invalid_argument("Cannot find codec");
	}

	m_codecContext = avcodec_alloc_context3(m_codec);

	if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0) {
		throw std::invalid_argument("Cannot open context");
	}

	m_frame = allocate_frame();

	return true;
}

void AudioDecoder::teardown() {
	assert(m_codecContext);
	assert(m_codec);
	assert(m_frame);

	//av_freep(m_frame->data[0]);
	free_frame(&m_frame);

	avcodec_close(m_codecContext);
	free_codec_context(m_codecContext);

	m_codecContext = nullptr;
	m_codec = nullptr;
	m_frame = nullptr;
}

bool AudioDecoder::decode(void* inputData, size_t inputLen, void* data, size_t len) {
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = static_cast<uint8_t*>(inputData);
	packet.size = static_cast<int>(inputLen);
	auto result = decode(&packet, data, len);
	free_packet(&packet);
	return result;
}

bool AudioDecoder::decode(AVPacket* packet, void* data, size_t len) {

	int gotFrame;
	if (avcodec_decode_audio4(m_codecContext, m_frame, &gotFrame, packet) >= 0) {
		if (!data)
			return false;
		if (gotFrame) {
			auto data_size = av_samples_get_buffer_size(nullptr, m_codecContext->channels, m_frame->nb_samples, m_codecContext->sample_fmt, 1);
			memcpy(data, m_frame->data[0], MIN(len, data_size));
			return true;
		}
	}
	else {
		std::cout << packet->stream_index << " ERROR" << std::endl;
	}
	return false;
}

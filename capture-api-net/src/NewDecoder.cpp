#include <assert.h>

#include "NewDecoder.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#ifdef USE_IPP
#include <ipps.h>
#endif
#include <iostream>

extern "C"
{
#include "./libavformat/avformat.h"
#include "./libavcodec/avcodec.h"
#include "./libswscale/swscale.h"
#include "./libavutil/imgutils.h"
}

NewDecoder::NewDecoder() :
m_codecContext(nullptr),
m_codec(nullptr),
m_swsContext(nullptr),
m_frame(nullptr),
m_scaledFrame(nullptr){

}

NewDecoder::~NewDecoder(){

}

void NewDecoder::setup(AVCodecID codecId)
{
	assert(!m_codecContext);
	assert(!m_codec);
	assert(!m_swsContext);
	assert(!m_frame);
	assert(!m_scaledFrame);

	m_codec = avcodec_find_decoder(codecId);

	if (!m_codec)
	{
		std::cout << "Cannot find codec" << std::endl;
		abort();
	}

	m_codecContext = avcodec_alloc_context3(m_codec);
	m_codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

	if (m_codec->capabilities&CODEC_CAP_TRUNCATED)
		m_codecContext->flags |= CODEC_FLAG_TRUNCATED;

	if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0)
	{
		std::cout << "Cannot open context" << std::endl;
		abort();
	}

	m_frame = avcodec_alloc_frame();
	setupScale();
}

void NewDecoder::setupScale()
{
	m_swsContext = sws_getCachedContext(m_swsContext, width(), height(), m_codecContext->pix_fmt, width(), height(), AV_PIX_FMT_BGR24, SWS_BICUBIC, nullptr, nullptr, nullptr);

	if (!m_swsContext)
	{
		std::cout << "Cannot get context" << std::endl;
		abort();
	}


	m_scaledFrame = avcodec_alloc_frame();

	if (av_image_alloc(m_scaledFrame->data, m_scaledFrame->linesize, width(), height(), AV_PIX_FMT_BGR24, 1) < 0)
	{
		std::cout << "Cannot allocate image" << std::endl;
		abort();
	}
}

int NewDecoder::width()
{
	return m_frame->width ? m_frame->width : 640;
}

int NewDecoder::height()
{
	return m_frame->height ? m_frame->height : 480;
}

void NewDecoder::setup(AVCodecContext* codecContext)
{
	assert(codecContext);

	assert(!m_codecContext);
	assert(!m_codec);
	assert(!m_swsContext);
	assert(!m_frame);
	assert(!m_scaledFrame);

	m_codecContext = codecContext;
	m_codec = avcodec_find_decoder(m_codecContext->codec_id);
	if (!m_codec)
	{
		std::cout << "Cannot find codec" << std::endl;
		abort();
	}

	if (avcodec_open2(m_codecContext, m_codec, nullptr) < 0)
	{
		std::cout << "Cannot open context" << std::endl;
		abort();
	}

	m_frame = avcodec_alloc_frame();

	setupScale();
}

void NewDecoder::teardown()
{
	assert(m_codecContext);
	assert(m_codec);
	assert(m_swsContext);
	assert(m_frame);
	assert(m_scaledFrame);

	//av_freep(m_frame->data[0]);
	avcodec_free_frame(&m_frame);

	av_freep(&m_scaledFrame->data[0]);
	avcodec_free_frame(&m_scaledFrame);

	avcodec_close(m_codecContext);
	sws_freeContext(m_swsContext);

	m_codecContext = nullptr;
	m_codec = nullptr;
	m_swsContext = nullptr;
	m_frame = nullptr;
	m_scaledFrame = nullptr;

}

bool NewDecoder::decode(void* inputData, size_t inputLen, void* data, size_t len)
{
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = static_cast<uint8_t*>(inputData);
	packet.size = static_cast<int>(inputLen);
	auto result = decode(&packet, data, len);
	av_free_packet(&packet);
	return result;
}

bool NewDecoder::decode(AVPacket* packet, void* data, size_t len)
{
	assert(m_swsContext != nullptr);

	int gotFrame;
	if (avcodec_decode_video2(m_codecContext, m_frame, &gotFrame, packet) >= 0)
	{
		if (!data) return false;
		if (gotFrame)
		{
			if (sws_scale(m_swsContext, m_frame->data, m_frame->linesize, 0, m_frame->height, m_scaledFrame->data, m_scaledFrame->linesize) < 0)
			{
				std::cout << "cannot scale in decode" << std::endl;
				abort();// TODO Brutal
			}
			else
			{
				memcpy(data, m_scaledFrame->data[0], MIN(len, m_codecContext->width * m_codecContext->height * 3));
			}

			return true;
		}
	}
	else
	{
		std::cout << packet->stream_index << " ERROR" << std::endl;
	}
	return false;
}

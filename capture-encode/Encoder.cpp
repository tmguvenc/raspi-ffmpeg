/*
 * Encoder.cpp
 *
 *  Created on: 6 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "Encoder.h"

extern "C" {
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
}
#include <string>
#include <iostream>

Encoder::Encoder() :
		m_pCodec(nullptr), m_pCodecContext(nullptr), m_scaleContext(nullptr), m_frame(nullptr), m_scaledFrame(nullptr) {

}

Encoder::~Encoder() {

}

bool Encoder::init(AVCodecID codecId, int width, int height, int quality) {
	m_pCodec = avcodec_find_encoder(codecId);

	if (!m_pCodec)
		return false;

	m_pCodecContext = avcodec_alloc_context3(m_pCodec);
	if (!m_pCodecContext)
		return false;

	std::string qualityStr;

	switch (quality) {
	case 0:
		qualityStr = "ultrafast";
		break;
	case 1:
		qualityStr = "superfast";
		break;
	case 2:
		qualityStr = "veryfast";
		break;
	case 3:
		qualityStr = "faster";
		break;
	case 4:
		qualityStr = "fast";
		break;
	case 5:
		qualityStr = "medium";
		break;
	case 6:
		qualityStr = "slow";
		break;
	case 7:
		qualityStr = "slower";
		break;
	case 8:
		qualityStr = "veryslow";
		break;
	default:
		return false;
	}

	if (codecId == AV_CODEC_ID_H264) {
		if (av_opt_set(m_pCodecContext->priv_data, "preset", qualityStr.c_str(), 0) < 0)
			return false;
	}

	m_pCodecContext->time_base = {1,25};
	m_pCodecContext->width = width;
	m_pCodecContext->height = height;
	m_pCodecContext->gop_size = 15;
	m_pCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
	//m_pCodecContext->thread_count = 4;

	if (avcodec_open2(m_pCodecContext, m_pCodec, NULL) < 0) {
		std::cout << "cannot open codec" << std::endl;
		return false;
	}

	m_scaleContext = sws_getContext(width, height, AV_PIX_FMT_YUYV422, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
	NULL, NULL, NULL);

	if (!m_scaleContext) {
		std::cout << "cannot initialize scale context" << std::endl;
		return false;
	}

	m_scaledFrame = av_frame_alloc();
	m_frame = av_frame_alloc();

	if (av_image_alloc(m_scaledFrame->data, m_scaledFrame->linesize, width, height, AV_PIX_FMT_YUV420P, 1) < 0) {
		std::cout << "Cannot allocate image" << std::endl;
		return false;
	}

	m_scaledFrame->format = AV_PIX_FMT_YUV420P;
	m_scaledFrame->width = width;
	m_scaledFrame->height = height;

	return true;
}

void Encoder::reset() {
	av_freep(&m_frame->data[0]);
	av_frame_free(&m_frame);

	av_freep(&m_scaledFrame->data[0]);
	av_frame_free(&m_scaledFrame);

	sws_freeContext(m_scaleContext);
	avcodec_close(m_pCodecContext);
	av_free(m_pCodecContext);
}

spEncodedBuffer Encoder::encode(void* inputData, size_t inputLen) {

	avpicture_fill((AVPicture *) m_frame, static_cast<uint8_t*>(inputData), AV_PIX_FMT_YUYV422, 640, 480);

	// rescale to outStream format
	int h = sws_scale(m_scaleContext, ((AVPicture*) m_frame)->data, ((AVPicture*) m_frame)->linesize, 0, 480,
			((AVPicture*) m_scaledFrame)->data, ((AVPicture*) m_scaledFrame)->linesize);

	if (h == 0) {
		fprintf(stderr, "Fail to rescale\n");
		return false;
	}

	AVFrame scaledFrame;

	AVPacket pkt;
	if (!encode(scaledFrame, pkt)) {
		av_packet_unref(&pkt);
		return false;
	}

	auto encodedBuffer = std::make_shared<EncodedBuffer>(pkt.size);

	memcpy(encodedBuffer->buffer(), pkt.data, encodedBuffer->len());

	av_packet_unref(&pkt);

	return encodedBuffer;
}

bool Encoder::encode(AVFrame &frame, AVPacket &pkt) {
	av_init_packet(&pkt);
	pkt.data = nullptr;
	pkt.size = 0;

	int nGotPacket = -1;

	static auto index = 0;

	m_scaledFrame->pts = index++;

	if (avcodec_encode_video2(m_pCodecContext, &pkt, m_scaledFrame, &nGotPacket) < 0)
		return false;

	if (nGotPacket < 1)
		return false;

	if (m_pCodecContext->coded_frame->key_frame)
		pkt.flags |= AV_PKT_FLAG_KEY;

	return true;
}

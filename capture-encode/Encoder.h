/*
 * Encoder.h
 *
 *  Created on: 6 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef ENCODER_H_
#define ENCODER_H_

extern "C" {
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

class Encoder {
public:
	Encoder();
	~Encoder();

private:
	AVCodec *m_pCodec;
	AVCodecContext *m_pCodecContext;
	SwsContext *m_scaleContext;
	AVFrame* m_frame;
	AVFrame* m_scaledFrame;

public:
	bool init(AVCodecID codecId, int width, int height, int quality);
	void reset();
	bool encode(AVFrame &frame, AVPacket &pkt);
	bool encode(void* inputData, size_t inputLen, void*& data, size_t& len);
};

#endif /* ENCODER_H_ */

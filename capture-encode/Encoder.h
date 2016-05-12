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

#include <malloc.h>
#include <memory>

class EncodedBuffer{
public:
	EncodedBuffer(size_t lenght) : m_lenght(lenght){
		m_buffer = malloc(lenght);
	}
	~EncodedBuffer(){
		free(m_buffer);
	}

	void* buffer(){
		return m_buffer;
	}

	size_t len(){
		return m_lenght;
	}
private:
	void* m_buffer;
	size_t m_lenght;
};

typedef std::shared_ptr<EncodedBuffer> spEncodedBuffer;

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
	spEncodedBuffer encode(void* inputData, size_t inputLen);
};

#endif /* ENCODER_H_ */

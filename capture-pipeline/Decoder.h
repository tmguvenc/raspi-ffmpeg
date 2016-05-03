/*
 * Decoder.h
 *
 *  Created on: 2 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef DECODER_H_
#define DECODER_H_

#include <stdlib.h>

struct AVPacket;
struct AVCodecContext;
struct AVFormatContext;
struct SwsContext;
struct AVFrame;
struct AVCodec;

extern "C"
{
	#include <libavcodec/avcodec.h>
}

class Decoder
{
public:
public:
	Decoder();
	virtual ~Decoder();

	bool decode(AVPacket* packet, void* data, size_t len);
	bool decode(void* inputData, size_t inputLen, void* data, size_t len);
	void setup(AVCodecID codecId);
	void setupScale();
	void teardown();
	int width();
	int height();

private:
	AVCodecContext* m_codecContext;
	AVCodec* m_codec;
	SwsContext* m_swsContext;
	AVFrame* m_frame;
	AVFrame* m_scaledFrame;
};

#endif /* DECODER_H_ */

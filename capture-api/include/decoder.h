/*
 * Decoder.h
 *
 *  Created on: 2 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef DECODER_H_
#define DECODER_H_

#include <stdlib.h>
#include <capture_macros.h>

struct AVPacket;
struct AVCodecContext;
struct AVFormatContext;
struct SwsContext;
struct AVFrame;
struct AVCodec;
class ICapture;

extern "C"
{
	#include <libavcodec/avcodec.h>
}

class CAPTURE_EXPORT Decoder
{
public:
	Decoder(int destWidth, int destHeight);
	virtual ~Decoder();

	bool decode(AVPacket* packet, void* data, size_t len);
	bool decode(void* inputData, size_t inputLen, void* data, size_t len);
	bool setup(AVCodecID codecId, AVPixelFormat pix_fmt);
	bool setup(ICapture* capture);
	bool setupScale();
	void teardown();
	int width();
	int height();

private:
	AVCodecContext* m_codecContext;
	AVCodec* m_codec;
	SwsContext* m_swsContext;
	AVFrame* m_frame;
	AVFrame* m_scaledFrame;
	int m_destWidth, m_destHeight;
};

#endif /* DECODER_H_ */

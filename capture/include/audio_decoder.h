/*
 * AudioDecoder.h
 *
 *  Created on: 2 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef AUDIO_DECODER_H_
#define AUDIO_DECODER_H_

#include <stdlib.h>
#include <capture_macros.h>

struct AVPacket;
struct AVCodecContext;
struct AVFormatContext;
struct AVFrame;
struct AVCodec;
class ICapture;

extern "C"
{
	#include <libavcodec/avcodec.h>
}

class CAPTURE_EXPORT AudioDecoder
{
public:
	AudioDecoder();
	virtual ~AudioDecoder();

	bool decode(AVPacket* packet, void* data, size_t len);
	bool decode(void* inputData, size_t inputLen, void* data, size_t len);
	bool setup(AVCodecID codecId);
	void teardown();

private:
	AVCodecContext* m_codecContext;
	AVCodec* m_codec;
	AVFrame* m_frame;
};

#endif /* AUDIO_DECODER_H_ */

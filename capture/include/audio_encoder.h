#ifndef AUDIO_ENCODER_H
#define AUDIO_ENCODER_H

#include <stdint.h>
#include <capture_macros.h>

struct AVCodecContext;
struct AVFrame;
struct AVCodec;
struct AVPacket;

class CAPTURE_EXPORT AudioEncoder
{
public:
	AudioEncoder();
	~AudioEncoder();

	bool encode(void* inputData, size_t inputLen, void* data, size_t& len);

private:
	AVCodec *m_pCodec;
	AVCodecContext *m_pCodecContext;
	AVFrame* m_frame;
	int m_buffer_size;
};

#endif
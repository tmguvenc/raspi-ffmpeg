#ifndef NEW_DECODER_H
#define NEW_DECODER_H

struct AVPacket;
struct AVCodecContext;
struct AVFormatContext;
struct SwsContext;
struct AVFrame;
struct AVCodec;
enum AVCodecID;

class NewDecoder
{
public:
	bool decode(AVPacket* packet, void* data, size_t len);
	bool decode(void* inputData, size_t inputLen, void* data, size_t len);
	NewDecoder();
	~NewDecoder();
	void setup(AVCodecContext* codecContext);
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

#endif
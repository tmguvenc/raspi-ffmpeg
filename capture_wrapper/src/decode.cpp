#include <decode.h>

Decode::Decode(int width, int height) :
m_decoder(width, height)
{
	m_decoder.setup(AV_CODEC_ID_MJPEG, AV_PIX_FMT_YUV422P);
	m_internalBuffer.resize(width * height * 3);
}

Decode::~Decode()
{
	m_decoder.teardown();
}

// TODO
std::vector<unsigned char> Decode::decode(const std::vector<unsigned char>& buffer)
{
	m_decoder.decode(const_cast<unsigned char*>(&buffer[0]), buffer.size(), &m_internalBuffer[0], m_internalBuffer.size());

	return m_internalBuffer;
}
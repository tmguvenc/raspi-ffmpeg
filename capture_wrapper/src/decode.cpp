#include <decode.h>
#include <iostream>

Decode::Decode(int width, int height) :
m_decoder(width, height)
{
	m_decoder.setup(AV_CODEC_ID_MJPEG, AV_PIX_FMT_YUV422P);
}

Decode::~Decode()
{
	m_decoder.teardown();
}

void Decode::decode(const std::vector<unsigned char>& buffer, char *BYTE)
{
	if (!m_decoder.decode(const_cast<unsigned char*>(&buffer[0]), buffer.size(), reinterpret_cast<unsigned char*>(BYTE), 1280*720*3))
	{
		std::cout << "cannot decode!\n";
	}
}
#ifndef DECODE_H
#define DECODE_H

#include <decoder.h>
#include <vector>

class Decode
{
public:
	explicit Decode(int width, int height);
	~Decode();

	std::vector<unsigned char> decode(const std::vector<unsigned char>& buffer);

private:
	Decoder m_decoder;
	std::vector<unsigned char> m_internalBuffer;
};

#endif // !DECODE_H
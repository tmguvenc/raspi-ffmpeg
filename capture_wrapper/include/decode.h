#ifndef DECODE_H
#define DECODE_H

#include <decoder.h>
#include <vector>

class Decode
{
public:
	explicit Decode(int width, int height);
	~Decode();

	void decode(const std::vector<unsigned char>& buffer, char *BYTE);

private:
	Decoder m_decoder;
};

#endif // !DECODE_H
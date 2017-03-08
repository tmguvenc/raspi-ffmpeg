#ifndef FRAME_H
#define FRAME_H

#include <data.h>

class VideoFrame : public Data
{
public:
	explicit VideoFrame(uint32_t size) : Data(size)
	{
		
	}
	VideoFrame(void* data, uint32_t size, uint32_t index) : Data(size), m_index(index) {
		memcpy(m_data, data, size);
	}

	inline uint32_t getIndex() const { return m_index; }

private:
	uint32_t m_index;
};

#endif
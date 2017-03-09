#ifndef FRAME_H
#define FRAME_H

#include <data.h>
#include <cstring>

class VideoFrame : public Data
{
public:
	explicit VideoFrame(uint32_t size) : Data(size)
	{
		static_assert(std::is_base_of<Data, VideoFrame>::value, 
			"Video Frame must be derived from Data.");
	}
	VideoFrame(void* data, uint32_t size, uint32_t index) : Data(size), m_index(index) {
		memcpy(m_data, data, size);
	}

	inline uint32_t getIndex() const { return m_index; }

private:
	uint32_t m_index;
};

#endif

#include <video_frame.h>

VideoFrame::VideoFrame(uint32_t size) : Data(size)
{
	static_assert(std::is_base_of<Data, VideoFrame>::value,
		"Video Frame must be derived from Data.");
}

VideoFrame::VideoFrame(void* data, uint32_t size, uint32_t index) : Data(size), m_index(index) {
	memcpy(m_data, data, size);
}
uint32_t VideoFrame::getIndex() const
{
	return m_index;
}

int VideoFrame::type()
{
	return 1;
}
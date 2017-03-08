#ifndef FRAME_H
#define FRAME_H

#include <malloc.h>
#include <memory>

class VideoFrame
{
public:
	VideoFrame() : m_size(0), m_data(nullptr){}

	VideoFrame(void* data, size_t size, size_t index) : m_size(size), m_index(index)
	{
		m_data = malloc(size);
		memcpy(m_data, data, size);
	}
	~VideoFrame()
	{
		if (m_data)
		{
			free(m_data);
			m_data = nullptr;
			m_size = 0;
		}
	}

	size_t m_size;
	size_t m_index;
	void* m_data;

private:
	VideoFrame& operator=(const VideoFrame& other);
	VideoFrame(const VideoFrame& other);
};

typedef std::shared_ptr<VideoFrame> spVideoFrame;

#endif
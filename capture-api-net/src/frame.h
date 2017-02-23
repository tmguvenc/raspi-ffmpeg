#ifndef FRAME_H
#define FRAME_H

#include <malloc.h>
#include <memory>

class Frame
{
public:
	Frame() : m_size(0), m_data(nullptr){}

	Frame(void* data, size_t size, size_t index) : m_size(size), m_index(index)
	{
		m_data = malloc(size);
		memcpy(m_data, data, size);
	}
	~Frame()
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
	Frame& operator=(const Frame& other);
	Frame(const Frame& other);
};

typedef std::shared_ptr<Frame> spFrame;

#endif
#ifndef DATA_H
#define DATA_H

#include <memory>
#include <common_macros.h>

class COMMON_EXPORT Data
{
public:
	explicit Data(uint32_t size) : m_size(size)
	{
		m_data = malloc(m_size);
	}

	virtual ~Data() {
		if (m_data)
		{
			free(m_data);
			m_data = nullptr;
			m_size = 0;
		}
	}

	virtual int type() = 0;

	void* getData() const { return m_data; }
	uint32_t getSize() const { return m_size; }

protected:
	uint32_t m_size;
	void* m_data;

private:
	Data& operator=(const Data& other);
	Data(const Data& other);
};

#endif
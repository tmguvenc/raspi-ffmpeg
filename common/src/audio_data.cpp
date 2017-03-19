#include <audio_data.h>
#include <cstring>

AudioData::AudioData(uint32_t size) : Data(size)
{
	static_assert(std::is_base_of<Data, AudioData>::value,
		"Audio Data must be derived from Data.");
}

AudioData::AudioData(const void* data, uint32_t size) : Data(size)
{
	memcpy(m_data, data, size);
}

int AudioData::type()
{
	return 0;
}

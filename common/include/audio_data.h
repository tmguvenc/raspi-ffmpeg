#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <data.h>

class COMMON_EXPORT AudioData : public Data
{
public:
	explicit AudioData(uint32_t size);
	AudioData(const void* data, uint32_t size);
	int type() override;
};

#endif

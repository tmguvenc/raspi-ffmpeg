#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <data.h>
#include <cstring>
#include <common_macros.h>

class COMMON_EXPORT VideoFrame : public Data
{
public:
	explicit VideoFrame(uint32_t size);
	VideoFrame(void* data, uint32_t size, uint32_t index);
	inline uint32_t getIndex() const;
	int type() override;

private:
	uint32_t m_index;
};

#endif

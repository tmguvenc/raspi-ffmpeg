/*
 * VAFrame.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef FRAME_H
#define FRAME_H

#include <frame_container.h>

struct AVPacket;

class CAPTURE_EXPORT Frame: public FrameContainer {
public:
	Frame(int width, int height, int channels, uint64_t frameCount);
	~Frame() override;

	inline int getWidth() override
	{
		return m_width;
	}
	inline int getHeight() override
	{
		return m_height;
	}
	inline int getChannels() override
	{
		return m_channels;
	}
	inline uint64_t getFrameIndex() override
	{
		return m_index;
	}
	inline uint64_t getFrameCount() override
	{
		return m_frameCount;
	}
	inline AVPacket* getPacket() {
		return m_packet;
	}

	uint64_t getEncodedLength() override;

	bool isKey() override;

	bool decodeFrame(void*, uint64_t) override;

	uint64_t size() override;

	void* data() override;

private:
	AVPacket* m_packet;
	uint64_t m_index;
	int m_width;
	int m_height;
	int m_channels;
	uint64_t m_frameCount;
};

#endif /* VAFRAME_H_ */

/*
 * AudioFrame.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef AUDIO_FRAME_H
#define AUDIO_FRAME_H

#include <frame_container.h>
#include <capture_macros.h>

struct AVPacket;

class CAPTURE_EXPORT AudioFrame: public FrameContainer {
public:
	explicit AudioFrame(uint64_t frameCount);
	~AudioFrame() override;

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

#endif /* AUDIO_FRAME_H_ */

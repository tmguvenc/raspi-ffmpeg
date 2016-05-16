/*
 * VAFrame.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VAFRAME_H_
#define VAFRAME_H_

#include "VAFrameContainer.h"

struct AVPacket;

class VAFrame: public VAFrameContainer {
public:
	VAFrame(int width, int height, int channels, size_t frameCount);
	~VAFrame() override;

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
	inline size_t getFrameIndex() override
	{
		return m_index;
	}
	inline size_t getFrameCount() override
	{
		return m_frameCount;
	}
	inline AVPacket* getPacket() {
		return m_packet;
	}

	size_t getEncodedLength() override;
	bool isKey() override;
	bool decodeFrame(void*, size_t) override;

	size_t size() override;
	void* data() override;

private:
	AVPacket* m_packet;
	size_t m_index;
	int m_width;
	int m_height;
	int m_channels;
	size_t m_frameCount;
};

#endif /* VAFRAME_H_ */

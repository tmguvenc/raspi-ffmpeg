/*
 * VAFrame.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "VAFrame.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

VAFrame::VAFrame(int width, int height, int channels, size_t frameCount) :
		m_packet(new AVPacket()), m_index(0), m_width(width), m_height(height), m_channels(channels), m_frameCount(
				frameCount) {
	av_init_packet(m_packet);
}

VAFrame::~VAFrame() {
	av_packet_free(&m_packet);
	delete m_packet;
}

bool VAFrame::isKey() {
	return m_packet->flags & AV_PKT_FLAG_KEY;
}

bool VAFrame::decodeFrame(void* data, size_t len) {
	return false;
}

size_t VAFrame::getEncodedLength() {
	return m_packet->size;
}

size_t VAFrame::size() const {
	return m_packet->size;
}

void* VAFrame::data() const {
	return m_packet->data;
}

/*
 * Frame.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <frame.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

Frame::Frame(int width, int height, int channels, uint64_t frameCount) :
		m_packet(new AVPacket()), m_index(0), m_width(width), m_height(height), m_channels(channels), m_frameCount(
				frameCount) {
	av_init_packet(m_packet);
}

Frame::~Frame() {

#if defined(_WIN32) && defined(_MSC_VER)
	av_free_packet(m_packet);
#else
	av_packet_unref(m_packet);
#endif
	delete m_packet;
}

bool Frame::isKey() {
	return m_packet->flags & AV_PKT_FLAG_KEY;
}

bool Frame::decodeFrame(void* data, uint64_t len) {
	return false;
}

uint64_t Frame::getEncodedLength() {
	return m_packet->size;
}

uint64_t Frame::size() {
	return m_packet->size;
}

void* Frame::data() {
	return m_packet->data;
}

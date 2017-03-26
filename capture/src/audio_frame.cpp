/*
 * AudioFrame.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <audio_frame.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

AudioFrame::AudioFrame(uint64_t frameCount) :
		m_packet(new AVPacket()), m_index(0), m_frameCount(
				frameCount) {
	av_init_packet(m_packet);
}

AudioFrame::~AudioFrame() {

#if defined(_WIN32) && defined(_MSC_VER)
	av_free_packet(m_packet);
#else
	av_packet_unref(m_packet);
#endif
	delete m_packet;
}

bool AudioFrame::isKey() {
	return m_packet->flags & AV_PKT_FLAG_KEY;
}

bool AudioFrame::decodeFrame(void* data, uint64_t len) {
	return false;
}

uint64_t AudioFrame::getEncodedLength() {
	return m_packet->size;
}

uint64_t AudioFrame::size() {
	return m_packet->size;
}

void* AudioFrame::data() {
	return m_packet->data;
}

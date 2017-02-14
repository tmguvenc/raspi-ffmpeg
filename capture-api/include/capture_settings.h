/*
 * CaptureSettings.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef CAPTURESETTINGS_H_
#define CAPTURESETTINGS_H_

#include <capture_macros.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

class CAPTURE_EXPORT CaptureSettings 
{
public:
	CaptureSettings() :
			m_width(640), m_height(480), m_channels(3), m_fps(15), m_codecId(AV_CODEC_ID_MJPEG) {

	}
	CaptureSettings(uint32_t width, uint32_t height, uint32_t channels, uint32_t fps, AVCodecID codecId) :
			m_width(width), m_height(height), m_channels(channels), m_fps(fps), m_codecId(codecId) {

	}

	uint32_t getWidth() {
		return m_width;
	}
	uint32_t getHeight() {
		return m_height;
	}
	uint32_t getChannels() {
		return m_channels;
	}
	uint32_t getFPS() {
		return m_fps;
	}
	AVCodecID getCodecId() {
		return m_codecId;
	}

private:

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_channels;
	uint32_t m_fps;
	AVCodecID m_codecId;
};

#endif /* CAPTURESETTINGS_H_ */

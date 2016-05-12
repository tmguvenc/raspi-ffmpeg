/*
 * CaptureSettings.h
 *
 *  Created on: 12 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef CAPTURESETTINGS_H_
#define CAPTURESETTINGS_H_

extern "C" {
#include <libavcodec/avcodec.h>
}

struct CaptureSettings {
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	uint32_t fps;
	AVCodecID codecId;
};

#endif /* CAPTURESETTINGS_H_ */

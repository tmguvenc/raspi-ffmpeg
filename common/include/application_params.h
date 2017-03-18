#ifndef APPLICATION_PARAMS_H
#define	APPLICATION_PARAMS_H

#include <string>
#include <vector>
#include <stdint.h>

struct ApplicationParams
{
	uint16_t port;
	uint8_t fps;
	uint32_t width;
	uint32_t height;
	uint32_t codec;
	uint32_t delayMicroseconds;
	uint32_t step;
	uint16_t max_videoframe_queue_size;
	uint16_t max_audioframe_queue_size;
	uint8_t sensorPin;
	std::vector<uint8_t> panMotorPins;
	std::vector<uint8_t> tiltMotorPins;
	std::string video_source_url;
	std::string audio_source_url;
};

#endif
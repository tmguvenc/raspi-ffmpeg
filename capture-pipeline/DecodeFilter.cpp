/*
 * DecodeFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "DecodeFilter.h"
#include "VAFrame.h"
#include <opencv2/opencv.hpp>

#define WIDTH 640
#define HEIGHT 480

const auto len = HEIGHT * WIDTH * 3;

DecodeFilter::DecodeFilter() :
Filter(parallel)
{
	decoder.setup(AV_CODEC_ID_MJPEG);
}

DecodeFilter::~DecodeFilter()
{
	decoder.teardown();
}

void* DecodeFilter::operator ()(void* ptr){
	VAFrame* frame = static_cast<VAFrame*>(ptr);

	if (frame->size() == 0) return nullptr;

	cv::Mat *image = new cv::Mat(HEIGHT, WIDTH, CV_8UC3);

	decoder.decode(frame->data(), frame->size(), image->data, len);

	delete frame;

	return image;
}

void DecodeFilter::finalize(void* ptr){

}

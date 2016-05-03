/*
 * DecodeFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "DecodeFilter.h"
#include "VAFrame.h"
#include <opencv2/opencv.hpp>
#include "MatrixPool.h"

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
	auto image = ObjectPool::pool_mat()->create({HEIGHT, WIDTH, CV_8UC3});
	decoder.decode(frame->data(), frame->size(), image->data, len);
	delete frame;
	return new std::shared_ptr<cv::Mat>(image);
}

void DecodeFilter::finalize(void* ptr){

}

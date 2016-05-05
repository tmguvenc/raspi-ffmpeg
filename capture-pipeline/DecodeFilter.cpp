/*
 * DecodeFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "DecodeFilter.h"
#include "VAFrame.h"
#include <opencv2/opencv.hpp>
#include "FrameContext.h"

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

void* DecodeFilter::operator ()(void* userData){

	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	VAFrame* frame = static_cast<VAFrame*>(context->m_rawFrame);
	if (frame->size() == 0) return nullptr;
	decoder.decode(frame->data(), frame->size(), context->m_frame_org->data, len);
	delete frame;

	return userData;
}

void DecodeFilter::finalize(void* userData){
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
}

/*
 * GrayscaleFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "GrayscaleFilter.h"
#include <opencv2/opencv.hpp>
#include "FrameContext.h"
#include <time.h>
#include <chrono>
#include <thread>

GrayscaleFilter::GrayscaleFilter() :
Filter(parallel)
{
	srand (time(NULL));
}

GrayscaleFilter::~GrayscaleFilter()
{
}

void* GrayscaleFilter::operator ()(void* userData){
	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	cv::cvtColor(*context->m_frame_org, *context->m_frame_gray, CV_BGR2GRAY);

	return userData;
}

void GrayscaleFilter::finalize(void* userData){
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
}

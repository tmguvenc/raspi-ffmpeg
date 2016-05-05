/*
 * OutputFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "OutputFilter.h"
#include <opencv2/opencv.hpp>
#include "FrameContext.h"

OutputFilter::OutputFilter():
Filter(serial)
{
	cv::namedWindow("image");
	cv::namedWindow("image_gray");
}

OutputFilter::~OutputFilter()
{
	cv::destroyAllWindows();
}

void *OutputFilter::operator()(void *userData){
	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	cv::putText(*context->m_frame_org, std::to_string(context->m_frameIndex), {30, context->m_frame_org->rows - 30}, CV_FONT_HERSHEY_SIMPLEX, 0.6, {0,255,0}, 1, CV_AA);

	cv::imshow("image", *context->m_frame_org);
	cv::imshow("image_gray", *context->m_frame_gray);
	cv::waitKey(1);

	delete c;

	return nullptr;
}

void OutputFilter::finalize(void *userData){
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
}

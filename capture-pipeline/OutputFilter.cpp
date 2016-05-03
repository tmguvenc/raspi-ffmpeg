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
}

OutputFilter::~OutputFilter()
{
	cv::destroyAllWindows();
}

void *OutputFilter::operator()(void *userData){
	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	cv::imshow("image", *context->m_frame_gray);
	cv::waitKey(1);

	delete c;

	return nullptr;
}

void OutputFilter::finalize(void *){

}

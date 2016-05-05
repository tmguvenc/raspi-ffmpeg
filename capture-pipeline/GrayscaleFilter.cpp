/*
 * GrayscaleFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "GrayscaleFilter.h"
#include <opencv2/opencv.hpp>
#include "FrameContext.h"

GrayscaleFilter::GrayscaleFilter() :
		Filter(parallel)
{

}

GrayscaleFilter::~GrayscaleFilter()
{
}

void* GrayscaleFilter::operator ()(void* userData)
{
	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	//cv::cvtColor(*context->m_frame_org, *context->m_frame_gray, CV_BGR2GRAY); // hangs here

	auto org_data = context->m_frame_org->data;
	auto gray_data = context->m_frame_gray->data;

	auto rows = context->m_frame_org->rows;
	auto cols = context->m_frame_org->cols;

	auto index_gray = 0;
	auto index_org = 0;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			gray_data[index_gray++] = org_data[index_org++] * 0.114f + org_data[index_org++] * 0.587f + org_data[index_org++] * 0.299f;

	return userData;
}

void GrayscaleFilter::finalize(void* userData)
{
	std::cout << "finalize: GrayscaleFilter" << std::endl;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
	context->m_operationHandle.Done();
}

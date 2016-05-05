/*
 * OutputFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "OutputFilter.h"
#include <opencv2/opencv.hpp>

OutputFilter::OutputFilter(tbb::concurrent_bounded_queue<spFrameContext>* output_queue):
Filter(serial),
m_output_queue(output_queue)
{

}

OutputFilter::~OutputFilter()
{

}

void *OutputFilter::operator()(void *userData){
	if (!userData) return nullptr;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;

	cv::putText(*context->m_frame_org, std::to_string(context->m_frameIndex), {30, context->m_frame_org->rows - 30}, CV_FONT_HERSHEY_SIMPLEX, 0.6, {0,255,0}, 1, CV_AA);

	m_output_queue->push(context);

	return nullptr;
}

void OutputFilter::finalize(void *userData){
	std::cout << "finalize: OutputFilter" << std::endl;
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
	context->m_operationHandle.Done();
}

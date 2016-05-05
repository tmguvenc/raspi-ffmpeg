/*
 * InputFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "InputFilter.h"

InputFilter::InputFilter(tbb::concurrent_bounded_queue<spFrameContext>* queue) :
Filter(serial),
m_queue(queue)
{
}

InputFilter::~InputFilter()
{
}

void* InputFilter::operator ()(void* userData){
	spFrameContext ctx;
	m_queue->pop(ctx);
	return new spFrameContext(ctx);
}

void InputFilter::finalize(void* userData){
}

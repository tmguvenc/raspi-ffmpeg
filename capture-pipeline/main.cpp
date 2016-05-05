/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "WebcamCapture.h"
#include "DecodeFilter.h"
#include "OutputFilter.h"
#include "GrayscaleFilter.h"
#include "InputFilter.h"
#include "ContextPool.h"

tbb::concurrent_bounded_queue<spFrameContext> context_queue;
auto frameIndex = 0;
ContextPool contextPool(50);

void callback(void* ptr){
	auto context = contextPool.CreateFrameContext();
	context->m_frameIndex = ++frameIndex;
	context->m_rawFrame = static_cast<VAFrameContainer*>(ptr);
	std::cout << contextPool.getBufferSize() << std::endl;
	context_queue.push(context);
}

int main()
{
	context_queue.set_capacity(20);

	WebcamCapture capture("/dev/video0");
	capture.init();

	InputFilter inputFilter(&context_queue);
	DecodeFilter decodeFilter;
	GrayscaleFilter grayscaleFilter;
	OutputFilter outputFilter;

	tbb::pipeline pipeline;

	pipeline.add_filter(inputFilter);
	pipeline.add_filter(decodeFilter);
	pipeline.add_filter(grayscaleFilter);
	pipeline.add_filter(outputFilter);

	capture.start(callback);

	pipeline.run(100);

	return 0;
}

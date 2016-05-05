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
tbb::concurrent_bounded_queue<spFrameContext> output_queue;
auto frameIndex = 0;
ContextPool contextPool(50);

int main()
{
	context_queue.set_capacity(20);
	output_queue.set_capacity(20);

	WebcamCapture capture("/dev/video0");
	capture.init();

	InputFilter inputFilter(&context_queue);
	DecodeFilter decodeFilter;
	GrayscaleFilter grayscaleFilter;
	OutputFilter outputFilter(&output_queue);

	tbb::pipeline pipeline;

	pipeline.add_filter(inputFilter);
	pipeline.add_filter(decodeFilter);
	pipeline.add_filter(grayscaleFilter);
	pipeline.add_filter(outputFilter);

	capture.start([](void* ptr){
		auto context = contextPool.CreateFrameContext();
		context->m_frameIndex = ++frameIndex;
		context->m_rawFrame = static_cast<VAFrameContainer*>(ptr);
		context_queue.push(context);
	});

	tbb::tbb_thread thread([](){
		cv::namedWindow("image");
		cv::namedWindow("image_gray");
		cv::moveWindow("image", 100, 100);
		cv::moveWindow("image_gray", 740, 100);

		while(true){
			spFrameContext context;
			output_queue.pop(context);

			cv::imshow("image", *context->m_frame_org);
			cv::imshow("image_gray", *context->m_frame_gray);
			cv::waitKey(1);
			context.reset();
		}

		cv::destroyAllWindows();
	});

	pipeline.run(100);

	return 0;
}

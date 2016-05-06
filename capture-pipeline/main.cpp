/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "WebcamCapture.h"
#include "DecodeFilter.h"
#include "GaborFilter.h"
#include "OutputFilter.h"
#include "GrayscaleFilter.h"
#include "InputFilter.h"
#include "ContextPool.h"
#include <chrono>
#include <thread>

tbb::concurrent_bounded_queue<spFrameContext> context_queue;
tbb::concurrent_bounded_queue<spFrameContext> output_queue;
auto frameIndex = 0;
ContextPool contextPool(20);

inline void drawRatioBar(cv::Mat& mat, double ratio)
{
	auto barHeight = static_cast<int>(mat.rows * ratio) - 13;

	cv::Rect borderRect(5, 3, 20, mat.rows - 6);

	cv::Rect barRect(8, mat.rows - barHeight - 5, 14, barHeight);

	auto r = static_cast<int>(255 * ratio);
	auto g = static_cast<int>(255 * std::abs<double>(1.0 - ratio));

	cv::rectangle(mat, borderRect, cv::Scalar(128, 128, 0), 1);

	cv::rectangle(mat, barRect, cv::Scalar(0, g, r), -1);
}

int main()
{
	context_queue.set_capacity(20);
	output_queue.set_capacity(20);

	WebcamCapture capture("/dev/video0");
	capture.init();

	InputFilter inputFilter(&context_queue);
	DecodeFilter decodeFilter;
	GrayscaleFilter grayscaleFilter;
	GaborFilter gaborFilter;
	OutputFilter outputFilter(&output_queue);

	tbb::pipeline pipeline;

	pipeline.add_filter(inputFilter);
	pipeline.add_filter(decodeFilter);
	pipeline.add_filter(grayscaleFilter);
	pipeline.add_filter(gaborFilter);
	pipeline.add_filter(outputFilter);

	capture.start([](void* ptr){
		auto context = contextPool.CreateFrameContext();
		context->m_frameIndex = ++frameIndex;
		context->m_ratio = contextPool.getBufferSize();
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

			std::cout << context->m_ratio * 20 << std::endl;

			drawRatioBar(*context->m_frame_org, context->m_ratio);
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

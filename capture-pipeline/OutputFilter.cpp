/*
 * OutputFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "OutputFilter.h"
#include <opencv2/opencv.hpp>

OutputFilter::OutputFilter():
Filter(serial)
{
	cv::namedWindow("image");
}

OutputFilter::~OutputFilter()
{
	cv::destroyAllWindows();
}

void *OutputFilter::operator()(void *image_ptr){

	auto image = static_cast<cv::Mat*>(image_ptr);

	cv::imshow("image", *image);
	cv::waitKey(1);

	delete image;
	return nullptr;
}

void OutputFilter::finalize(void *){

}

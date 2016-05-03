/*
 * OutputFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat G�ven�
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
	auto aa = static_cast<std::shared_ptr<cv::Mat>*>(image_ptr);
	auto image = *aa;
	delete aa;

	cv::imshow("image", *image);
	cv::waitKey(1);

	return nullptr;
}

void OutputFilter::finalize(void *){

}

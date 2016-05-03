/*
 * GrayscaleFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "GrayscaleFilter.h"
#include <opencv2/opencv.hpp>

GrayscaleFilter::GrayscaleFilter() :
Filter(parallel)
{

}

GrayscaleFilter::~GrayscaleFilter()
{
}

void* GrayscaleFilter::operator ()(void* image_ptr){

	auto image = static_cast<cv::Mat*>(image_ptr);

	cv::Mat *grayImage = new cv::Mat(image->rows, image->cols, CV_8UC1);

	cv::cvtColor(*image, *grayImage, CV_BGR2GRAY);

	delete image;

	return grayImage;
}

void GrayscaleFilter::finalize(void* ){

}

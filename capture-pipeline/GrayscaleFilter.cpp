/*
 * GrayscaleFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "GrayscaleFilter.h"
#include <opencv2/opencv.hpp>
#include "MatrixPool.h"

GrayscaleFilter::GrayscaleFilter() :
Filter(parallel)
{

}

GrayscaleFilter::~GrayscaleFilter()
{
}

void* GrayscaleFilter::operator ()(void* image_ptr){
	auto aa = static_cast<std::shared_ptr<cv::Mat>*>(image_ptr);
	auto image = *aa;
	delete aa;

	auto grayImage = ObjectPool::pool_mat()->create({image->rows, image->cols, CV_8UC1});
	cv::cvtColor(*image, *grayImage, CV_BGR2GRAY);
	return new std::shared_ptr<cv::Mat>(grayImage);
}

void GrayscaleFilter::finalize(void* ){

}

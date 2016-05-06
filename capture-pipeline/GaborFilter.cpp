/*
 * GaborFilter.cpp
 *
 *  Created on: 6 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "GaborFilter.h"
#include "MatrixPool.h"
#include "FrameContext.h"

GaborFilter::GaborFilter() :
Filter(parallel),
m_kernelSize(6), m_theta(4.0 * CV_PI / 180),
m_psi(156.0 * CV_PI / 180), m_lambda(0.5 + 20.0 / 100.0)
{
	if (!(m_kernelSize % 2))
		m_kernelSize = m_kernelSize + 1;

	m_kernel.create(m_kernelSize, m_kernelSize, CV_32F);
	m_sigma = 4.0 / static_cast<double>(m_kernelSize);

	computeKernel();

	m_element = cv::getStructuringElement(cv::MORPH_RECT, { 17*640/1024, 3*480/576 }); // element size affects performance
}

GaborFilter::~GaborFilter()
{
	m_kernel.release();
	m_element.release();
}

void *GaborFilter::operator ()(void *userData)
{
	if (!userData) return nullptr;

	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;

	{
		MatPoolKey key1(480, 640, CV_32F);
		MatPoolKey key2(480, 640, CV_8UC1);

		auto src_f = ObjectPool::pool_mat()->create(key1);
		auto destImage = ObjectPool::pool_mat()->create(key1);
		auto threshImage = ObjectPool::pool_mat()->create(key2);

		context->m_frame_gray->convertTo(*src_f, CV_32F, 1.0/255.0);
		cv::filter2D(*src_f, *destImage, CV_32F, m_kernel, cv::Point(-1, -1), 0, IPL_BORDER_REPLICATE);
		cv::pow(*destImage, 2, *src_f);

		for (auto i = 0; i < context->m_magnitudeImage->rows; ++i)
			memset((context->m_magnitudeImage->data + i * context->m_magnitudeImage->step1()), 0, (int)context->m_magnitudeImage->step1());

		src_f->convertTo(*context->m_magnitudeImage, CV_8U, 255);
		cv::adaptiveThreshold(*context->m_magnitudeImage, *threshImage, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 5, 13);
		cv::morphologyEx(*threshImage, *context->m_magnitudeImage, CV_MOP_CLOSE, m_element, cv::Point(-1, -1), 1, IPL_BORDER_REPLICATE);
		cv::erode(*context->m_magnitudeImage, *threshImage, m_element);
		cv::dilate(*threshImage, *context->m_magnitudeImage, m_element);
	}

	return userData;
}

void GaborFilter::finalize(void *userData)
{
	auto c = static_cast<spFrameContext*>(userData);
	auto context = *c;
	delete c;
	context->m_operationHandle.Done();
}

void GaborFilter::computeKernel()
{
	auto hks = (m_kernelSize - 1) / 2;
	auto del = 2.0 / (m_kernelSize - 1);

	double x_theta;
	double y_theta;

	auto cos_theta = cos(m_theta);
	auto sin_theta = sin(m_theta);
	auto pow_sigma = pow(m_sigma, 2);

	for (auto y = -hks; y <= hks; ++y)
	{
		auto y_del_sin = y*del*sin_theta;
		auto y_del_cos = y*del*cos_theta;

		for (auto x(-hks); x <= hks; ++x)
		{
			x_theta = x*del*cos_theta + y_del_sin;
			y_theta = -x*del*sin_theta + y_del_cos;
			m_kernel.at<float>(hks + y, hks + x) = static_cast<float>(exp(-0.5*(pow(x_theta, 2) + pow(y_theta, 2)) / pow_sigma)) * static_cast<float>(cos(2 * CV_PI * x_theta / m_lambda + m_psi));
		}
	}
}

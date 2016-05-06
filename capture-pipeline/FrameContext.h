/*
 * FrameContext.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef FRAMECONTEXT_H_
#define FRAMECONTEXT_H_

#include <opencv2/opencv.hpp>
#include <memory>
#include "OperationHandle.h"
#include "MatrixPool.h"

class VAFrameContainer;

class FrameContext
{
public:
	FrameContext() : m_frameIndex(0), m_rawFrame(nullptr), m_ratio(0.0f){
		m_frame_org =  ObjectPool::pool_mat()->create({480, 640, CV_8UC3});
		m_frame_gray = ObjectPool::pool_mat()->create({480, 640, CV_8UC1});
		m_magnitudeImage = ObjectPool::pool_mat()->create({480, 640, CV_8UC1});
	}
	~FrameContext(){
		m_frame_org->release();
		m_frame_gray->release();
		m_magnitudeImage->release();
	}

	std::shared_ptr<cv::Mat> m_frame_org, m_frame_gray, m_magnitudeImage;

	size_t m_frameIndex;
	VAFrameContainer* m_rawFrame;
	OperationHandle m_operationHandle;
	float m_ratio;
};

typedef std::shared_ptr<FrameContext> spFrameContext;


#endif /* FRAMECONTEXT_H_ */

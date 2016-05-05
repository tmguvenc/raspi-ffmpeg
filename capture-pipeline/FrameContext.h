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

class VAFrameContainer;

class FrameContext
{
public:
	FrameContext() : m_frameIndex(0), m_rawFrame(nullptr), m_ratio(0.0f){
		m_frame_org = std::make_shared<cv::Mat>(480, 640, CV_8UC3);
		m_frame_gray = std::make_shared<cv::Mat>(480, 640, CV_8UC1);
	}
	~FrameContext(){
		m_frame_org->release();
		m_frame_gray->release();
	}

	std::shared_ptr<cv::Mat> m_frame_org, m_frame_gray;

	size_t m_frameIndex;
	VAFrameContainer* m_rawFrame;
	OperationHandle m_operationHandle;
	float m_ratio;
};

typedef std::shared_ptr<FrameContext> spFrameContext;


#endif /* FRAMECONTEXT_H_ */

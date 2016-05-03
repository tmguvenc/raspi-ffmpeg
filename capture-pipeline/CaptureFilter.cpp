/*
 * CaptureFilter.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "CaptureFilter.h"
#include "WebcamCapture.h"

CaptureFilter::CaptureFilter(const std::string& connectionString) :
Filter(serial),
m_capture(new WebcamCapture(connectionString)),
m_contextPool(50)
{
	m_capture->init();
}

CaptureFilter::~CaptureFilter()
{
	m_capture->teardown();
	delete m_capture;
	m_capture = nullptr;
}

void* CaptureFilter::operator ()(void*){
	auto context = m_contextPool.CreateFrameContext();
	context->m_rawFrame = m_capture->grabFrame();
	std::cout << "ratio: " << m_contextPool.getBufferSize() << std::endl;
	return new spFrameContext(context);
}

void CaptureFilter::finalize(void* ptr){

}

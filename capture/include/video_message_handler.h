#ifndef VIDEO_MESSAGE_HANDLER_H
#define VIDEO_MESSAGE_HANDLER_H

#include <messages.h>
#include <data.h>
#include <string>
#include <capture.h>
#include <capture_settings.h>
#include "frame.h"
#include <tbb/concurrent_queue.h>
#include <common_utils.h>
#include <video_frame.h>
#include <iostream>

class VideoMessageHandler
{
public:
	explicit VideoMessageHandler(const std::string& url, int width, int height, int codec, int fps) :
		m_settings(width, height, 3, fps, static_cast<AVCodecID>(codec))
	{
		m_capture = m_captureFactory.create(url);
		m_capture->init(&m_settings);
		m_capture->startAsync([this](void* ptr) {
			m_frameQueue.push(static_cast<Frame*>(ptr));
		});
	}

	~VideoMessageHandler()
	{
		std::cout << "destroying VideoMessageHandler" << std::endl;
		clearQueue(&m_frameQueue);
	}

	Data* execute(const MessageType& message_type)
	{
		Data* data = nullptr;

		if (message_type == FrameRequest)
		{
			FrameContainer* frame;
			m_frameQueue.pop(frame);
			assert(frame != nullptr && "frame is null");
			data = new VideoFrame(frame->data(), frame->size(), 0);
			delete frame;
		}

		return data;
	}
private:
	std::unique_ptr<ICapture, CaptureStopper<ICapture>> m_capture;
	WebcamCaptureFactory m_captureFactory;
	CaptureSettings m_settings;
	tbb::concurrent_bounded_queue<FrameContainer*> m_frameQueue;
};

#endif
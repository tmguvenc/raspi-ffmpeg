#ifndef AUDIO_MESSAGE_HANDLER_H
#define AUDIO_MESSAGE_HANDLER_H

#include <messages.h>
#include <data.h>
#include <string>
#include <capture.h>
#include "frame.h"
#include <tbb/concurrent_queue.h>
#include <common_utils.h>
#include <video_frame.h>
#include <iostream>
#include <audio_capture_factory.h>
#include "audio_frame.h"
#include <audio_data.h>
#include <audio_encoder.h>

class AudioMessageHandler
{
public:
	explicit AudioMessageHandler(const std::string& url, uint32_t queueCapacity)
	{
		m_frameQueue.set_capacity(queueCapacity);
		m_capture = m_captureFactory.create(url);
		m_capture->init(nullptr);
		m_capture->startAsync([this](void* ptr) {
			m_frameQueue.push(static_cast<AudioFrame*>(ptr));
		});

		m_buffer = malloc(10000);
	}

	~AudioMessageHandler()
	{
		m_capture.reset();
		clearQueue(&m_frameQueue);
		free(m_buffer);
	}

	Data* execute(const MessageType& message_type)
	{
		if (message_type != AudioFrameRequest)
			return nullptr;

		FrameContainer* frame;
		m_frameQueue.pop(frame);
		assert(frame != nullptr && "audio frame is null");

		size_t dataSize;

		if (!m_encoder.encode(frame->data(), frame->size(), m_buffer, dataSize))
			return nullptr;

		auto data = new AudioData(m_buffer, dataSize);
		delete frame;

		return data;
	}
private:
	std::unique_ptr<ICapture, CaptureStopper<ICapture>> m_capture;
	AudioCaptureFactory m_captureFactory;
	tbb::concurrent_bounded_queue<FrameContainer*> m_frameQueue;
	AudioEncoder m_encoder;

	void* m_buffer;
};

#endif

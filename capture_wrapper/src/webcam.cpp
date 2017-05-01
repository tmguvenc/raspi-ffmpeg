#include <webcam.h>
#include <webcam_capture.h>
#include <capture_settings.h>
extern "C" {
#include <libavcodec/avcodec.h>
}
#include <frame_container.h>
#include <tbb/concurrent_queue.h>

using QueueType = tbb::concurrent_bounded_queue<FrameContainer*>;
QueueType queue;

Webcam::Webcam(const std::string& url, int width, int height, int codec, int fps)
{
	queue.set_capacity(20);

	m_capture = m_factory.create(url);
	CaptureSettings settings(width, height, 3, fps, static_cast<AVCodecID>(codec));
	m_capture->init(&settings);
	m_capture->startAsync([](FrameContainer* frm)
	{
		queue.push(frm);
	});
}

Webcam::~Webcam()
{
	m_capture->stop();
}

std::vector<unsigned char> Webcam::getFrame()
{
	FrameContainer* fc;

	queue.pop(fc);

	std::vector<unsigned char> buffer(fc->size());
	memcpy(&buffer[0], fc->data(), fc->size());
	delete fc;

	return std::move(buffer);
}
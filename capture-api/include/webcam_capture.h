/*
 * VACapture.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef WEBCAMCAPTURE_H_
#define WEBCAMCAPTURE_H_

#include <future>
#include <capture_base.h>
#include <assert.h>

struct AVFormatContext;
struct AVCodecContext;
struct AVDictionary;

class FrameContainer;

namespace spdlog
{
	class logger;
}

class WebcamCapture: public ICapture {
	friend class WebcamCaptureFactory;
public:
	WebcamCapture(const WebcamCapture&) = delete;
	WebcamCapture& operator=(const WebcamCapture&) = delete;
	virtual ~WebcamCapture();

	void init(CaptureSettings* settings) override;
	void start(CaptureCallback func) override;
	void startAsync(CaptureCallback func) override;
	void stop() override;
	bool started() override;
	bool completed() override;
	void* getCodecInfo() override;


	inline CaptureSettings* getSettings() override{
		assert(m_settings != nullptr);
		return m_settings;
	}
protected:
	FrameContainer* grabFrame();
	explicit WebcamCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger> logger);

private:
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;bool m_completed;
	std::atomic<bool> m_run;
	int m_indexofVideoStream;

	std::future<void> m_captureHandle;
	AVFormatContext* m_formatContext;
	AVDictionary* m_options;
	AVCodecContext* m_codecContext;

	std::shared_ptr<spdlog::logger> m_logger;
};

#endif /* WEBCAMCAPTURE_H_ */

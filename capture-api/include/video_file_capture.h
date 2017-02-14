/*
 * VideoFileCapture.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VIDEOFILECAPTURE_H_
#define VIDEOFILECAPTURE_H_

#include <future>
#include <capture_base.h>

struct AVFormatContext;
struct AVCodecContext;
struct AVDictionary;

class FrameContainer;

namespace spdlog
{
	class logger;
}

class VideoFileCapture: public ICapture {
public:
	friend class VideoFileCaptureFactory;
public:
	VideoFileCapture(const VideoFileCapture&) = delete;
	VideoFileCapture& operator=(const VideoFileCapture&) = delete;
	virtual ~VideoFileCapture();

	void init(CaptureSettings* settings) override;
	void start(CaptureCallback func) override;
	void startAsync(CaptureCallback func) override;
	void stop() override;bool completed() override;
	void* getCodecInfo() override;

	CaptureSettings* getSettings() override{
		return m_settings;
	}
protected:
	FrameContainer* grabFrame();
	explicit VideoFileCapture(const std::string& connectionString, std::shared_ptr<spdlog::logger> logger);

private:
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;bool m_completed;
	volatile bool m_run;
	int m_indexofVideoStream;

	std::future<void> m_captureHandle;
	AVFormatContext* m_formatContext;
	AVCodecContext* m_codecContext;
	std::shared_ptr<spdlog::logger> m_logger;
};

#endif /* VIDEOFILECAPTURE_H_ */

/*
 * VideoFileCapture.h
 *
 *  Created on: 13 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VIDEOFILECAPTURE_H_
#define VIDEOFILECAPTURE_H_

#include "ICapture.h"
#include <future>

struct AVFormatContext;
struct AVCodecContext;
struct AVDictionary;

class VAFrameContainer;
struct CaptureSettings;

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

protected:
	VAFrameContainer* grabFrame();

private:
	explicit VideoFileCapture(const std::string& connectionString);
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;bool m_completed;
	volatile bool m_run;
	int m_indexofVideoStream;

	std::future<void> m_captureHandle;
	AVFormatContext* m_formatContext;
	AVCodecContext* m_codecContext;
};

#endif /* VIDEOFILECAPTURE_H_ */

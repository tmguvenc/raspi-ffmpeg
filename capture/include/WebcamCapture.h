/*
 * VACapture.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef WEBCAMCAPTURE_H_
#define WEBCAMCAPTURE_H_

#include "ICapture.h"
#include <future>

struct AVFormatContext;
struct AVCodecContext;
struct AVDictionary;

class VAFrameContainer;
struct CaptureSettings;

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
	bool completed() override;
	void* getCodecInfo() override;

protected:
	VAFrameContainer* grabFrame();
	explicit WebcamCapture(const std::string& connectionString);

private:
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;bool m_completed;
	volatile bool m_run;
	int m_indexofVideoStream;

	std::future<void> m_captureHandle;
	AVFormatContext* m_formatContext;
	AVDictionary* m_options;
	AVCodecContext* m_codecContext;
	CaptureSettings* m_settings;
};

#endif /* WEBCAMCAPTURE_H_ */

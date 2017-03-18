/*
 * VACapture.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef AUDIO_CAPTURE_H_
#define AUDIO_CAPTURE_H_

#include <future>
#include <capture_base.h>
#include <assert.h>

struct AVFormatContext;
struct AVCodecContext;
struct AVDictionary;

class FrameContainer;

class AudioCapture: public ICapture {
	friend class AudioCaptureFactory;
public:
	AudioCapture(const AudioCapture&) = delete;
	AudioCapture& operator=(const AudioCapture&) = delete;
	virtual ~AudioCapture();

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
	explicit AudioCapture(const std::string& connectionString);

private:
	std::string m_connectionString;
	bool m_completed;
	std::atomic<bool> m_run;
	int m_indexofVideoStream;

	std::future<void> m_captureHandle;
	AVFormatContext* m_formatContext;
	AVDictionary* m_options;
	AVCodecContext* m_codecContext;
};

#endif /* WEBCAMCAPTURE_H_ */

/*
 * VACapture.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef WEBCAMCAPTURE_H_
#define WEBCAMCAPTURE_H_

#include <string>
#include <tbb/tbb_thread.h>
#include "ICapture.h"

struct AVFormatContext;
struct AVDictionary;

class VAFrameContainer;

class WebcamCapture: public ICapture
{
public:
	WebcamCapture(const std::string& connectionString);
	virtual ~WebcamCapture();

	void init() override;
	void start(CaptureCallback func) override;
	void stop() override;
	void teardown() override;

	inline bool completed() const
	{
		return m_completed;
	}

protected:
	VAFrameContainer* grabFrame();

private:
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;
	bool m_completed;
	bool m_run;
	int m_indexofVideoStream;

	tbb::tbb_thread *m_thread;
	AVFormatContext* m_formatContext;
	AVDictionary *m_options;
};

#endif /* WEBCAMCAPTURE_H_ */

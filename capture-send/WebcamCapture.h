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
#include <tbb/concurrent_queue.h>
#include "ICapture.h"

struct AVCodecContext;
struct AVFormatContext;
struct SwsContext;
struct AVFrame;
struct AVCodec;

class VAFrameContainer;

class WebcamCapture: public ICapture
{
public:
	WebcamCapture(const std::string& connectionString,
			tbb::concurrent_bounded_queue<VAFrameContainer*>* queue);
	virtual ~WebcamCapture();

	void init() override;
	void start() override;
	void teardown() override;

	inline size_t getFrameCount() const
	{
		return m_totalFrameCount;
	}
	inline size_t frameIndex() const
	{
		return m_frameIndex;
	}
	inline bool completed() const
	{
		return m_completed;
	}
	inline AVFormatContext* getFormatContext() const
	{
		return m_formatContext;
	}

protected:
	VAFrameContainer* grabFrame();

private:
	std::string m_connectionString;
	int m_width;
	int m_height;
	int m_channels;
	size_t m_totalFrameCount;
	size_t m_frameIndex;
	bool m_completed;

	int m_indexofVideoStream;

	double m_timeBaseMultiplier;
	double m_framePeriod;

	tbb::tbb_thread *m_thread;
	AVFormatContext* m_formatContext;
	tbb::concurrent_bounded_queue<VAFrameContainer*>* m_queue;
};

#endif /* WEBCAMCAPTURE_H_ */

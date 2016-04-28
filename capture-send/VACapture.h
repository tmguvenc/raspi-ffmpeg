/*
 * VACapture.h
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VACAPTURE_H_
#define VACAPTURE_H_

#include <string>
#include <tbb/tbb_thread.h>
#include <tbb/concurrent_queue.h>

struct AVCodecContext;
struct AVFormatContext;
struct SwsContext;
struct AVFrame;
struct AVCodec;

class VAFrameContainer;

class VACapture
{
public:
	VACapture(const std::string& connectionString,
			tbb::concurrent_bounded_queue<VAFrameContainer*>* queue);
	~VACapture();

	void start();
	void teardown();

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
	int m_width;
	int m_height;
	int m_channels;
	int m_indexofVideoStream;
	double m_framePeriod;
	double m_timeBaseMultiplier;
	size_t m_totalFrameCount;
	size_t m_frameIndex;
	bool m_completed;

	AVFormatContext* m_formatContext;
	tbb::tbb_thread *m_thread;
	tbb::concurrent_bounded_queue<VAFrameContainer*>* m_queue;
};

#endif /* VACAPTURE_H_ */

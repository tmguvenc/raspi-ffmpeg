/*
 * VideoFrameSender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef VIDEO_FRAME_SENDER_H_
#define VIDEO_FRAME_SENDER_H_

#include <functional>
#include <comm_macros.h>

class FrameContainer;
typedef std::function<FrameContainer*(void)> DataSupplier;

class SenderPrivate;

class COMMUNICATION_EXPORT VideoFrameSender
{
public:
	explicit VideoFrameSender(int port, int width, int height, int codec);
	virtual ~VideoFrameSender();

	void start(DataSupplier ds);
	void stop();

private:
	SenderPrivate* m_ptr;
};

#endif /* SENDER_H_ */

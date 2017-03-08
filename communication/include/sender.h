/*
 * Sender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <functional>
#include <comm_macros.h>

class FrameContainer;
typedef std::function<FrameContainer*(void)> DataSupplier;

class CaptureSettings;
class SenderPrivate;

class COMMUNICATION_EXPORT Sender
{
public:
	explicit Sender(int port, const CaptureSettings& settings);
	virtual ~Sender();

	void start(DataSupplier ds);
	void stop();

private:
	SenderPrivate* m_ptr;
};

#endif /* SENDER_H_ */

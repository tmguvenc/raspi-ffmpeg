/*
 * Sender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <tbb/concurrent_queue.h>
#include "VAFrame.h"

class Sender
{
public:
	Sender(int port, tbb::concurrent_bounded_queue<VAFrameContainer*>* queue);
	virtual ~Sender();

	void start();
	void stop();

private:
	int m_port;
	void* m_context;
	void* m_socket;
	tbb::concurrent_bounded_queue<VAFrameContainer*>* m_queue;
	bool m_run;
	char m_buffer[10];
};

#endif /* SENDER_H_ */

/*
 * Sender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef SENDER_H_
#define SENDER_H_

#include "VAFrame.h"

typedef VAFrameContainer*(*DataSupplier)();

class Sender
{
public:
	Sender(int port);
	virtual ~Sender();

	void start(DataSupplier ds);
	void stop();

private:
	int m_port;
	void* m_context;
	void* m_socket;
	bool m_run;
	char m_buffer[10];
};

#endif /* SENDER_H_ */

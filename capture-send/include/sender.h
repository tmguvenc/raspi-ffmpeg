/*
 * Sender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat G�ven�
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <frame.h>
#include <functional>

typedef std::function<FrameContainer*(void)> DataSupplier;

class Sender
{
public:
	explicit Sender(int port);
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

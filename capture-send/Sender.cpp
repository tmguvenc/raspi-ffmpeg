/*
 * Sender.cpp
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include "Sender.h"
#include <zmq.h>
#include <sstream>
#include <iostream>

Sender::Sender(int port,
		tbb::concurrent_bounded_queue<VAFrameContainer*>* queue) :
		m_port(port), m_queue(queue), m_run(false)
{
	m_context = zmq_ctx_new();
	m_socket = zmq_socket(m_context, ZMQ_REP);

	std::stringstream url;
	url << "tcp://*:" << port;
	zmq_bind(m_socket, url.str().c_str());

	std::cout << "bound to " << url.str() << std::endl;

	memset(m_buffer, 0, sizeof m_buffer);
}

Sender::~Sender()
{
	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Sender::start()
{
	std::cout << "sender started" << std::endl;

	m_run = true;

	while (m_run)
	{
		// wait for new frame request
		zmq_recv(m_socket, m_buffer, sizeof m_buffer, 0);
		if (strncmp(m_buffer, "frame", 5) != 0)
			continue;

		// pop new frame from queue
		VAFrameContainer *f;
		m_queue->pop(f);

		// send frame
		auto frame = static_cast<VAFrame*>(f);
		zmq_send(m_socket, frame->data(), frame->size(), 0);

		// don't forget to release
		delete frame;
	}

	std::cout << "sender stopped" << std::endl;
}

void Sender::stop()
{
	m_run = false;
}

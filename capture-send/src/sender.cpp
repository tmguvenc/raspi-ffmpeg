/*
 * Sender.cpp
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <sender.h>
#include <zmq.h>
#include <sstream>
#include <iostream>
#include <string.h>

Sender::Sender(int port) :
		m_port(port), m_run(false)
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

void Sender::start(DataSupplier ds)
{
	std::cout << "sender started" << std::endl;

	m_run = true;

	while (m_run)
	{
		// wait for new frame request
		zmq_recv(m_socket, m_buffer, sizeof m_buffer, 0);
		if (strncmp(m_buffer, "frame", 5) != 0)
			continue;

		auto f = ds();

		if (f == nullptr)
			break;
		// send frame
		auto frame = static_cast<Frame*>(f);
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

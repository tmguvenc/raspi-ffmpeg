/*
 * Sender.cpp
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <sender.h>
#include <zmq.h>
#include <string>
#include <string.h>
#include <spdlog/details/spdlog_impl.h>

Sender::Sender(int port) :
		m_port(port), m_run(false),
		m_logger(spdlog::stdout_color_mt("sender"))
{
	m_context = zmq_ctx_new();
	m_socket = zmq_socket(m_context, ZMQ_ROUTER);

	auto val = 1;
	auto xx = zmq_setsockopt(m_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val);
	assert(xx == 0);

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	assert(r == 0);

	zmq_bind(m_socket, ("tcp://*:" + std::to_string(m_port)).c_str());

	m_logger->info("listening port {}", m_port);

	memset(m_buffer, 0, sizeof m_buffer);
}

Sender::~Sender()
{
	spdlog::drop("sender");

	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Sender::start(DataSupplier ds)
{
	m_logger->info("sender started");
	m_run = true;

	char client_id[80] = { 0 };

	while (m_run)
	{
		// get client identifier
		auto len_id = zmq_recv(m_socket, client_id, sizeof(client_id), 0);
		assert(len_id > 0);

		// read empty frame
		auto len_ef = zmq_recv(m_socket, m_buffer, sizeof(m_buffer), 0);
		assert(len_ef == 0);

		// wait for new frame request
		zmq_recv(m_socket, m_buffer, sizeof(m_buffer), 0);
		if (strncmp(m_buffer, "stop", 4) == 0)
		{
			m_logger->warn("received stop command");
			
			// send client id
			zmq_send(m_socket, client_id, sizeof(client_id), ZMQ_SNDMORE);

			// Send empty frame
			zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
			zmq_send(m_socket, "stopped", sizeof("stopped"), 0);
			m_run = false;
			continue;
		}

		if (strncmp(m_buffer, "frame", 5) != 0)
			continue;

		auto f = ds();

		if (f == nullptr)
			break;

		// send client id
		zmq_send(m_socket, client_id, len_id, ZMQ_SNDMORE);

		// Send empty frame
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);

		// send frame
		auto frame = static_cast<Frame*>(f);
		zmq_send(m_socket, frame->data(), frame->size(), 0);

		// don't forget to release
		delete frame;
	}

	m_logger->info("sender stopped");
}

void Sender::stop()
{
	
}

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
	m_socket = zmq_socket(m_context, ZMQ_REP);

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

	while (m_run)
	{
		// wait for new frame request
		zmq_recv(m_socket, m_buffer, sizeof m_buffer, 0);
		if (strncmp(m_buffer, "stop", 4) == 0)
		{
			m_logger->warn("received stop command");
			stop();
			continue;
		}

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

	m_logger->info("sender stopped");
}

void Sender::stop()
{
	m_run = false;
}

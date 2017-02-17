#include "connector.h"
#include <zmq.h>
#include <malloc.h>

using namespace Client;

Connector::Connector(const std::string& url, tbb::concurrent_bounded_queue<spFrame>* frame_queue, int width, int height) :
m_url(std::move(url)),
m_context(nullptr),
m_socket(nullptr),
m_frame_queue(frame_queue),
m_width(width),
m_height(height),
m_started(false)
{
	m_context = zmq_ctx_new();
	m_size = m_width * m_height * 3;
	m_buffer = malloc(m_size);
}

Connector::~Connector()
{
	if (m_socket) {
		zmq_disconnect(m_socket, m_url.c_str());
		zmq_close(m_socket);
		m_socket = nullptr;
	}
	if (m_context) {
		zmq_ctx_destroy(m_context);
		m_context = nullptr;
	}
	if (m_buffer) {
		free(m_buffer);
		m_buffer = nullptr;
	}
}

void Connector::start()
{
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	auto linger = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	zmq_connect(m_socket, m_url.c_str());
	uint32_t index = 0;
	m_started = true;
	while (m_started)
	{
		// Send empty frame
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		// Send data frame
		zmq_send(m_socket, "frame", 5, 0);

		// read empty frame
		zmq_recv(m_socket, m_buffer, m_size, 0);
		// read data
		auto recBytes = zmq_recv(m_socket, m_buffer, m_size, 0);
		m_frame_queue->push(std::make_shared<Frame>(m_buffer, recBytes, ++index));
	}
}

void Connector::stop()
{
	// Send empty frame
	zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
	// Send data frame
	zmq_send(m_socket, "stop", 4, 0);
	m_started  = false;
}
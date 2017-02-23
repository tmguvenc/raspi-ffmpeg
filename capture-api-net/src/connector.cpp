#include "connector.h"
#include <zmq.h>
#include <malloc.h>
#include <assert.h>
#include <vector>
#include <sstream>

using namespace Client;

inline std::string getHostName()
{
#if defined(_WIN32) && defined(_MSC_VER)
	unsigned long len = 1024;
	char buffer[1024];
	GetComputerName(buffer, &len);
	return std::string(buffer, 0, len);
#else
	char buffer[1024] = { 0 };
	auto ret = gethostname(buffer, 1024);
	return std::string(buffer);
#endif
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (std::getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

Connector::Connector(const std::string& url, tbb::concurrent_bounded_queue<spFrame>* frame_queue) :
m_url(std::move(url)),
m_context(nullptr),
m_socket(nullptr),
m_frame_queue(frame_queue),
m_width(-1),
m_height(-1),
m_codec(-1),
m_started(false)
{
	m_context = zmq_ctx_new();
	init();
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

void Connector::init()
{
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	auto linger = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.

	auto host = getHostName();
	zmq_setsockopt(m_socket, ZMQ_IDENTITY, host.c_str(), host.length());

	zmq_connect(m_socket, m_url.c_str());

	int init = 5;
	// Send empty frame
	zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
	// Send data frame
	zmq_send(m_socket, &init, sizeof(init), 0);

	char temp[80] = { 0 };

	// read empty frame
	auto r = zmq_recv(m_socket, temp, sizeof(temp), 0);
	assert(r == 0);
	r = zmq_recv(m_socket, temp, sizeof(temp), 0);
	assert(r == sizeof(init));

	// read empty frame
	r = zmq_recv(m_socket, temp, sizeof(temp), 0);
	assert(r == 0);
	r = zmq_recv(m_socket, temp, sizeof(temp), 0);

	auto v = split(std::string(temp, r), ',');
	m_width = std::atoi(v[0].c_str());
	m_height = std::atoi(v[1].c_str());
	m_codec = std::atoi(v[2].c_str());

	m_size = m_width * m_height * 3;
	m_buffer = malloc(m_size);
}

void Connector::start()
{
	int stop = 2;
	int nextFrame = 1;
	uint32_t index = 0;
	m_started = true;

	while (m_started)
	{
		// Send empty frame
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		// Send data frame
		zmq_send(m_socket, &nextFrame, sizeof(nextFrame), 0);

		// read empty frame
		zmq_recv(m_socket, m_buffer, m_size, 0);
		// read data
		auto recBytes = zmq_recv(m_socket, m_buffer, m_size, 0);
		m_frame_queue->push(std::make_shared<Frame>(m_buffer, recBytes, ++index));
	}

	// Send empty frame
	zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
	// Send data frame
	zmq_send(m_socket, &stop, sizeof(stop), 0);

	// read empty frame
	zmq_recv(m_socket, m_buffer, m_size, 0);
	// read data
	auto recBytes = zmq_recv(m_socket, m_buffer, m_size, 0);
}

void Connector::stop()
{
	m_started  = false;
}
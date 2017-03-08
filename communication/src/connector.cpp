#include "connector.h"
#include <zmq.h>
#include <assert.h>
#include <common_utils.h>
#include "messages.h"
#include "video_frame.h"

class ConnectorPrivate
{
public:
	explicit ConnectorPrivate(const std::string& url, tbb::concurrent_bounded_queue<spVideoFrame>* frame_queue) :
		m_url(std::move(url)),
		m_context(nullptr),
		m_socket(nullptr),
		m_width(-1),
		m_height(-1),
		m_codec(-1),
		m_frame_queue(frame_queue),
		m_started(false) {
		assert(m_frame_queue != nullptr);
		m_context = zmq_ctx_new();
		init();
	}
	~ConnectorPrivate() {
		destroy();
	}

	void start() {
		uint32_t index = 0;
		m_started = true;

		std::vector<char> buffer(m_size);

		while (m_started) {
			sendRequest(NextFrameRequest);

			// read empty frame
			zmq_recv(m_socket, &buffer[0], m_size, 0);
			// read data
			auto recBytes = zmq_recv(m_socket, &buffer[0], m_size, 0);

			// if it's a ping message
			if (recBytes == 4) continue;

			m_frame_queue->push(std::make_shared<VideoFrame>(&buffer[0], recBytes, ++index));
		}

		sendRequest(StopRequest);
	}

	void stop() {
		m_started = false;
	}

	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }
	inline int getCodec() { return m_codec; }

protected:
	void init() {
		m_socket = zmq_socket(m_context, ZMQ_DEALER);
		auto linger = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.

		auto host = getHostName();
		zmq_setsockopt(m_socket, ZMQ_IDENTITY, host.c_str(), host.length());

		zmq_connect(m_socket, m_url.c_str());

		sendRequest(Init);

		char temp[30] = { 0 };

		// read empty frame
		auto r = zmq_recv(m_socket, temp, sizeof(temp), 0);
		assert(r == 0);
		r = zmq_recv(m_socket, temp, sizeof(temp), 0);

		auto v = split(std::string(temp, r), ',');
		m_width = std::atoi(v[0].c_str());
		m_height = std::atoi(v[1].c_str());
		m_codec = std::atoi(v[2].c_str());

		m_size = m_width * m_height * 3;
	}

	void sendRequest(MessageType message) {
		// Send empty frame
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		// Send data frame
		zmq_send(m_socket, &message, sizeof(MessageType), 0);
	}

	void destroy() {
		if (m_socket) {
			zmq_disconnect(m_socket, m_url.c_str());
			zmq_close(m_socket);
			m_socket = nullptr;
		}
		if (m_context) {
			zmq_ctx_destroy(m_context);
			m_context = nullptr;
		}
	}

private:
	std::string m_url;
	void* m_context;
	void* m_socket;
	int m_width, m_height, m_codec;
	tbb::concurrent_bounded_queue<spVideoFrame>* m_frame_queue;
	bool m_started;
	int m_size;
};

Connector::Connector(const std::string& url, tbb::concurrent_bounded_queue<spVideoFrame>* frame_queue) :
m_ptr(new ConnectorPrivate(std::move(url), frame_queue)) {
}

Connector::~Connector() {
	if (m_ptr){
		delete m_ptr;
		m_ptr = nullptr;
	}
}

void Connector::start() {
	m_ptr->start();
}

void Connector::stop() {
	m_ptr->stop();
}

int Connector::getWidth() {
	return m_ptr->getWidth();
}

int Connector::getHeight() {
	return m_ptr->getHeight();
}

int Connector::getCodec() {
	return m_ptr->getCodec();
}
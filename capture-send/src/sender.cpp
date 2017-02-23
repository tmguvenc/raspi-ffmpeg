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
#include <chrono>
#include <thread>
#include <capture_settings.h>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS (3 * HEARTHBEAT_INTERVAL_IN_SECONDS)
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2

inline int64_t current_time(){
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return static_cast<int64_t>(temp) / 1000;
}

Sender::Sender(int port, const CaptureSettings& settings) :
m_port(port), m_run(false),
m_width(settings.getWidth()),
m_height(settings.getHeight()),
m_codec(settings.getCodecId()),
m_logger(spdlog::stdout_color_mt("sender")),
m_thread(nullptr)
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
	memset(m_client_id, 0, sizeof m_client_id);
}

Sender::~Sender()
{
	spdlog::drop("sender");

	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
	if (m_thread)
	{
		delete m_thread;
		m_thread = nullptr;
	}
}

void Sender::start(DataSupplier ds)
{
	m_logger->info("sender started");
	m_run = true;

	m_thread = new tbb::tbb_thread([](Sender* srv)
	{
		assert(srv != nullptr);
		while (srv->m_run)
			srv->poll(25);
	}, this);

	while (m_run)
	{
		Message message;
		m_message_queue.pop(message);

		switch (message.second)
		{
		case 1: // next frame
			{
				auto f = ds();
				if (f == nullptr)
					break;
				auto frame = static_cast<Frame*>(f);
				send(message.first, frame->data(), frame->size());

				delete frame;
			} break;
		case 2: // stop
			{
				auto ok = 0;
				send(message.first, &ok, sizeof(ok)); 
				remove(message.first);
				if (m_clients.empty())
					m_run = false;
			} break;
		case 5: // init
			{
				zmq_send(m_socket, message.first.c_str(), message.first.length(), ZMQ_SNDMORE);
				zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
				zmq_send(m_socket, &m_width, sizeof(m_width), ZMQ_SNDMORE);
				zmq_send(m_socket, &m_height, sizeof(m_height), ZMQ_SNDMORE);
				zmq_send(m_socket, &m_codec, sizeof(m_codec), 0);
			}break;
		}
	}

	m_logger->info("sender stopped");
}

void Sender::stop()
{

}

void Sender::poll(int timeout)
{
	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		receive();
	}

	auto currentTime = current_time();

	std::vector<std::string> names;
	auto ping = 1;

	std::for_each(m_clients.begin(), m_clients.end(), [&currentTime, &names, this, ping](const std::pair<std::string, CommTime>& pair)
	{
		auto time = pair.second;
		if (time.lastReceivedMessageTime >= 0 && currentTime - time.lastReceivedMessageTime > TIMEOUT_INTERVAL_IN_SECONDS){
			names.push_back(pair.first); // add this client to removal list
		}
		else if (time.lastSendMessageTime >= 0 && currentTime - time.lastSendMessageTime > HEARTHBEAT_INTERVAL_IN_SECONDS){
			send(pair.first, &ping, sizeof(ping));
		}
	});

	// Timeout this clients!
	for (const auto& name : names)
		remove(name);
}

void Sender::remove(const std::string& clientName)
{
	ClientMap::accessor ac;
	if (m_clients.find(ac, clientName))
	{
		if (m_logger)
			m_logger->warn("[{}] disconnected", clientName);

		m_clients.erase(ac);
	}
	ac.release();
}

void Sender::send(const std::string& clientName, const void* data, int size)
{
	ClientMap::accessor ac;
	if (m_clients.find(ac, clientName))
	{
		zmq_send(m_socket, clientName.c_str(), clientName.length(), ZMQ_SNDMORE);
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		zmq_send(m_socket, data, size, 0);

		ac->second.lastSendMessageTime = current_time();
	}
	else
	{
		if (m_logger)
			m_logger->warn("{} cannot be found", clientName);
	}
	ac.release();
}

void Sender::receive()
{
	// get client identifier
	auto len_id = zmq_recv(m_socket, m_client_id, sizeof(m_client_id), 0);
	assert(len_id > 0);

	// read empty frame
	auto len_ef = zmq_recv(m_socket, &m_commandId, sizeof(m_commandId), 0);
	assert(len_ef == 0);

	// wait for new frame request
	zmq_recv(m_socket, &m_commandId, sizeof(m_commandId), 0);

	std::string client(m_client_id, len_id);

	ClientMap::accessor ac;
	m_clients.insert(ac, client);
	ac->second.lastReceivedMessageTime = current_time();
	ac.release();

	m_message_queue.push(std::make_pair(client, m_commandId));
}

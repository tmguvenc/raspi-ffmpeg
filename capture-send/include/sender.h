/*
 * Sender.h
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <frame.h>
#include <functional>
#include <spdlog/logger.h>
#include <memory>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/tbb_thread.h>

typedef std::function<FrameContainer*(void)> DataSupplier;

struct CommTime
{
	int64_t lastReceivedMessageTime;
	int64_t lastSendMessageTime;
};

using ClientMap = tbb::concurrent_hash_map<std::string, CommTime>;
using Message = std::pair<std::string, int>;
using MessageQueue = tbb::concurrent_bounded_queue<Message>;

class CaptureSettings;

class Sender
{
public:
	explicit Sender(int port, const CaptureSettings& settings);
	virtual ~Sender();

	void start(DataSupplier ds);
	void stop();

protected:
	void poll(int timeout);
	void remove(const std::string& clientName);
	void receive();
	void send(const std::string& clientName, const void* data, int size);

private:
	int m_port;
	void* m_context;
	void* m_socket;
	bool m_run;
	char m_buffer[80];
	int m_commandId;
	char m_client_id[80];
	uint32_t m_width, m_height, m_codec;
	std::shared_ptr<spdlog::logger> m_logger;
	ClientMap m_clients;
	MessageQueue m_message_queue;
	tbb::tbb_thread *m_thread;
};

#endif /* SENDER_H_ */

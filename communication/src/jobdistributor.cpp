#include <zmq.h>
#include<jobdistributor.h>
#include <string>
#include <assert.h>
#include <algorithm>
#include <sensor_message_handler.h>
#include <motor_message_handler.h>
#include <video_message_handler.h>

struct CommunicationTime
{
	int64_t lastReceivedMessageTime;
	int64_t lastSendMessageTime;
};

JobDistributor::JobDistributor(int port, int width, int height, int codec, int fps, int delayMicroseconds, int step, const std::vector<int>& panPins, const std::vector<int>& tiltPins) :
m_motor_executer(new MotorMessageHandler(delayMicroseconds, step, std::move(panPins), std::move(tiltPins))),
m_sensor_executer(new SensorMessageHandler),
m_video_executer(new VideoMessageHandler("/dev/video0", width, height, codec, fps)),
m_motor_message_handler(&m_motor_request_queue, nullptr, m_motor_executer),
m_sensor_message_handler(&m_sensor_request_queue, &m_response_queue, m_sensor_executer),
m_video_message_handler(&m_video_request_queue, &m_response_queue, m_video_executer),
m_port(port),
m_context(nullptr),
m_socket(nullptr)
{
	if (width <= 1)
		throw std::invalid_argument("Width must be greater than 1");
	if (height <= 1)
		throw std::invalid_argument("Height must be greater than 1");
	if (codec <= 0)
		throw std::invalid_argument("Codec must be greater than 0");

	m_settings = std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(codec);
	memset(m_client_id, 0, sizeof(m_client_id));

	m_functionMap.resize(MessageCount, nullptr);

	m_functionMap[FrameRequest] = [this](const std::string& client){m_video_request_queue.push(std::make_pair(client, FrameRequest)); };
	m_functionMap[StopRequest] = [this](const std::string& client){m_video_request_queue.push(std::make_pair(client, StopRequest)); };
	m_functionMap[HumTempRequest] = [this](const std::string& client){m_sensor_request_queue.push(std::make_pair(client, HumTempRequest)); };
	m_functionMap[InitRequest] = [this](const std::string& client){
		auto message = InitResponse;
		zmq_send(m_socket, m_client_id, m_client_id_len, ZMQ_SNDMORE);
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		zmq_send(m_socket, &message, sizeof(MessageType), ZMQ_SNDMORE);
		zmq_send(m_socket, m_settings.c_str(), m_settings.size(), 0);
	};
	m_functionMap[MotorUpRequest] = [this](const std::string& client){m_motor_request_queue.push(std::make_pair(client, MotorUpRequest)); };
	m_functionMap[MotorDownRequest] = [this](const std::string& client){m_motor_request_queue.push(std::make_pair(client, MotorDownRequest)); };
	m_functionMap[MotorLeftRequest] = [this](const std::string& client){m_motor_request_queue.push(std::make_pair(client, MotorLeftRequest)); };
	m_functionMap[MotorRightRequest] = [this](const std::string& client){m_motor_request_queue.push(std::make_pair(client, MotorRightRequest)); };
}

JobDistributor::~JobDistributor()
{
	if (m_socket)
		zmq_close(m_socket);

	if (m_context)
		zmq_ctx_destroy(m_context);

	if (m_motor_executer) {
		delete m_motor_executer;
		m_motor_executer = nullptr;
	}
	if (m_sensor_executer) {
		delete m_sensor_executer;
		m_sensor_executer = nullptr;
	}
}

inline MessageType dummy(MessageType in)
{
	static std::vector<MessageType> cache = {
		FrameResponse,
		StopResponse,
		HumTempResponse,
		InitResponse
	};

	assert(in >= FrameRequest && in <= InitRequest);

	return cache[in];
}

void JobDistributor::start()
{
	init();

	m_sensor_message_handler.start();
	m_motor_message_handler.start();
	m_video_message_handler.start();

	m_run.store(true);

	m_thread = std::make_unique<tbb::tbb_thread>([this]()
	{
		while (m_run)
		{
			Response response;
			m_response_queue.pop(response);
			assert(response.second && "data is null");
			send(response.first.first, dummy(response.first.second), response.second->getData(), response.second->getSize());
			delete response.second;
		}
	});

	while (m_run)
		poll(10);

	m_motor_message_handler.stop();
	m_sensor_message_handler.stop();
}

void JobDistributor::stop()
{
	m_run.store(false);
}

void JobDistributor::init()
{
	m_context = zmq_ctx_new();

	if (!m_context)
		throw std::invalid_argument("Cannot create context");

	m_socket = zmq_socket(m_context, ZMQ_ROUTER);
	if (!m_socket)
		throw std::invalid_argument("Cannot create socket");

	auto val = 1;
	if (zmq_setsockopt(m_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val) != 0)
		throw std::invalid_argument(string_format("Cannot set socket option: %s", zmq_strerror(zmq_errno())));

	auto linger = 0;
	if (zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)) != 0) // close cagirildiktan sonra beklemeden socket'i kapat.
		throw std::invalid_argument(string_format("Cannot set socket option: %s", zmq_strerror(zmq_errno())));

	if (zmq_bind(m_socket, ("tcp://*:" + std::to_string(m_port)).c_str()) != 0)
		throw std::invalid_argument(string_format("cannot bind socket: %s", zmq_strerror(zmq_errno())));
}

void JobDistributor::receive()
{
	// get client identifier
	m_client_id_len = zmq_recv(m_socket, m_client_id, sizeof(m_client_id), 0);
	assert(m_client_id_len > 0);

	std::string client(m_client_id, m_client_id_len);

	MessageType message_type;
	// read empty frame
	auto len_ef = zmq_recv(m_socket, &message_type, sizeof(message_type), 0);
	assert(len_ef == 0);

	// wait for new frame request
	zmq_recv(m_socket, &message_type, sizeof(message_type), 0);

	assert(m_functionMap[message_type]);
	
	m_functionMap[message_type](client);

	ClientMap::accessor ac;
	m_clients.insert(ac, client);
	ac->second.lastReceivedMessageTime = current_time();
	ac.release();
}

void JobDistributor::poll(int timeout)
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
	const static auto ping = Ping;
	std::for_each(m_clients.begin(), m_clients.end(), [&currentTime, &names, this](const std::pair<std::string, CommunicationTime>& pair)
	{
		auto time = pair.second;
		if (time.lastReceivedMessageTime >= 0 && currentTime - time.lastReceivedMessageTime > TIMEOUT_INTERVAL_IN_SECONDS){
			names.push_back(pair.first); // add this client to removal list
		}
		else if (time.lastSendMessageTime >= 0 && currentTime - time.lastSendMessageTime > HEARTHBEAT_INTERVAL_IN_SECONDS){
			send(pair.first, Ping, &ping, sizeof(MessageType));
		}
	});

	// Timeout this clients!
	for (const auto& name : names)
		remove(name);
}

void JobDistributor::remove(const std::string& clientName)
{
	ClientMap::accessor ac;
	if (m_clients.find(ac, clientName))
		m_clients.erase(ac);
	ac.release();
	if (m_clients.empty())
		m_run = false;
}

void JobDistributor::send(const std::string& clientName, MessageType messageType, const void* data, int size)
{
	ClientMap::accessor ac;
	if (m_clients.find(ac, clientName))
	{
		zmq_send(m_socket, clientName.c_str(), clientName.length(), ZMQ_SNDMORE);
		zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
		zmq_send(m_socket, &messageType, sizeof(MessageType), ZMQ_SNDMORE);
		zmq_send(m_socket, data, size, 0);

		ac->second.lastSendMessageTime = current_time();
	}
	//else
	//{
	//	if (m_logger)
	//		m_logger->warn("{} cannot be found", clientName);
	//}
	ac.release();
}
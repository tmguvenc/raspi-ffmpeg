/*
 * VideoFrameSender.cpp
 *
 *  Created on: 28 Nis 2016
 *      Author: Turan Murat Güvenç
 */

#include <videoframesender.h>
#include <zmq.h>
#include <string>
#include <string.h>
#include <common_utils.h>
#include <frame.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/tbb_thread.h>
#include <messages.h>
#include <frame_container.h>
#include <iostream>
#include <hum_temp_sensor.h>
#include <sensor_data_base.h>
#include <step_motor_28byj48.h>

struct CommTime
{
	int64_t lastReceivedMessageTime;
	int64_t lastSendMessageTime;
};

using ClientMap = tbb::concurrent_hash_map<std::string, CommTime>;
using Message = std::pair<std::string, MessageType>;
using MessageQueue = tbb::concurrent_bounded_queue<Message>;

using ReadSensorCallback = std::function<void(std::unique_ptr<ISensorData>)>;

// bababababa
struct HumTempSensorManager
{
	explicit HumTempSensorManager(ReadSensorCallback cb) {
		auto thread = std::make_unique<tbb::tbb_thread>([cb](){
			HumidityTemperatureSensor sensor;
			cb(sensor.readData());
		});
	}
};

class SenderPrivate
{
public:
	explicit SenderPrivate(int port, int width, int height, int codec) :
		m_port(port), m_run(false),
		m_width(width),
		m_height(height),
		m_codec(codec),
		m_logger(spdlog::stdout_color_mt("sender")),
		m_thread(nullptr)
	{
		if (m_width <= 1)
			throw std::invalid_argument("Width must be greater than 1");
		if (m_height <= 1)
			throw std::invalid_argument("Height must be greater than 1");
		if (m_codec <= 0)
			throw std::invalid_argument("Codec must be greater than 0");

		m_settings = std::to_string(m_width) + "," + std::to_string(m_height) + "," + std::to_string(m_codec);
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

		if(zmq_bind(m_socket, ("tcp://*:" + std::to_string(m_port)).c_str())!= 0)
			throw std::invalid_argument(string_format("cannot bind socket: %s", zmq_strerror(zmq_errno())));

		m_logger->info("listening port {}", m_port);

		memset(m_client_id, 0, sizeof m_client_id);

		m_panMotor = new StepMotor28BYJ48(1000, 8);
		m_tiltMotor = new StepMotor28BYJ48(1000, 8);

		m_panMotor->setup({ 0, 2, 3, 4 });
		m_panMotor->stop();
	}

	virtual ~SenderPrivate()
	{
		spdlog::drop("sender");

		zmq_close(m_socket);
		zmq_ctx_destroy(m_context);
		if (m_thread)
		{
			delete m_thread;
			m_thread = nullptr;
		}

		if (m_panMotor) {
			m_panMotor->stop();
			delete m_panMotor;
			m_panMotor = nullptr;
		}
		if (m_tiltMotor) {
			m_tiltMotor->stop();
			delete m_tiltMotor;
			m_tiltMotor = nullptr;
		}
		assert(m_message_queue.empty());
	}

	void start(DataSupplier ds)
	{
		m_logger->info<std::string>("sender started");
		m_run = true;

		m_thread = new tbb::tbb_thread([](SenderPrivate* srv)
		{
			assert(srv != nullptr);
			while (srv->m_run)
				srv->poll(25);
		}, this);

		std::vector<std::function<bool(const std::string&)>> funcs =
		{
			[this, ds](const std::string& name) { //FrameRequest
				auto f = ds();
				if (f == nullptr)
					return false;
				auto frame = static_cast<Frame*>(f);
				send(name, FrameResponse, frame->data(), static_cast<int>(frame->size()));
				delete frame;
				return true;
			},
			[this](const std::string& name) { //StopRequest
				auto type = StopResponse;
				send(name, StopResponse, &type, sizeof(MessageType));
				remove(name);
				return m_run;
			},
			[this](const std::string& name) { //HumTempRequest
				HumTempSensorManager([this, &name](std::unique_ptr<ISensorData> data){
					auto h = data->getData(Humidity);
					auto t = data->getData(Temperature);
					const auto aa = std::to_string(h) + ";" + std::to_string(t);
					send(name, HumTempResponse, aa.c_str(), aa.size());
				});

				return true;
			},
			[this](const std::string&) { //InitRequest
				return true;
			},
			[this](const std::string& name) { //MotorUpRequest
				if (m_tiltMotor) {
					m_tiltMotor->move(true);
				}
				return true;
			},
			[this](const std::string& name) { //MotorDownRequest
				if (m_tiltMotor) {
					m_tiltMotor->move(false);
				}
				return true;
			},
			[this](const std::string& name) { //MotorRightRequest
				if (m_panMotor) {
					m_panMotor->move(true);
				}
				return true;
			},
			[this](const std::string& name) { //MotorLeftRequest
				if (m_panMotor) {
					m_panMotor->move(false);
				}
				return true;
			},
			[this](const std::string& name) { //MotorStopRequest
				if (m_panMotor) {
					m_panMotor->stop();
				}
				if (m_tiltMotor) {
					m_tiltMotor->stop();
				}
				return true;
			}
		};

		while (m_run)
		{
			Message message;
			m_message_queue.pop(message);

			if (message.second < 0 || message.second > funcs.size()) {
				throw std::out_of_range(string_format("Message type is invalid %d", message.second));
			}

			if (!funcs[message.second](message.first))
				break;
		}

		m_logger->info<std::string>("sender stopped");
	}

	void stop()
	{
	}

protected:
	void poll(int timeout)
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
		std::for_each(m_clients.begin(), m_clients.end(), [&currentTime, &names, this](const std::pair<std::string, CommTime>& pair)
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

	void remove(const std::string& clientName)
	{
		ClientMap::accessor ac;
		if (m_clients.find(ac, clientName))
		{
			if (m_logger)
				m_logger->warn("[{}] disconnected", clientName);

			m_clients.erase(ac);
		}
		ac.release();
		if (m_clients.empty())
			m_run = false;
	}

	void receive()
	{
		// get client identifier
		auto len_id = zmq_recv(m_socket, m_client_id, sizeof(m_client_id), 0);
		assert(len_id > 0);

		MessageType message_type;
		// read empty frame
		auto len_ef = zmq_recv(m_socket, &message_type, sizeof(message_type), 0);
		assert(len_ef == 0);

		// wait for new frame request
		zmq_recv(m_socket, &message_type, sizeof(message_type), 0);

		if (message_type == InitRequest)
		{
			auto message = InitResponse;
			zmq_send(m_socket, m_client_id, len_id, ZMQ_SNDMORE);
			zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
			zmq_send(m_socket, &message, sizeof(MessageType), ZMQ_SNDMORE);
			zmq_send(m_socket, m_settings.c_str(), m_settings.size(), 0);
			if (m_logger){
				m_logger->info("[{}] connected", std::string(m_client_id, len_id));
			}
		}

		std::string client(m_client_id, len_id);

		ClientMap::accessor ac;
		m_clients.insert(ac, client);
		ac->second.lastReceivedMessageTime = current_time();
		ac.release();

		m_message_queue.push(make_pair(client, message_type));
	}

	void send(const std::string& clientName, MessageType messageType, const void* data, int size)
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
		else
		{
			if (m_logger)
				m_logger->warn("{} cannot be found", clientName);
		}
		ac.release();
	}

private:
	int m_port;
	void* m_context;
	void* m_socket;
	bool m_run;
	char m_client_id[80];
	uint32_t m_width, m_height, m_codec;
	std::shared_ptr<spdlog::logger> m_logger;
	ClientMap m_clients;
	MessageQueue m_message_queue;
	tbb::tbb_thread *m_thread;
	std::string m_settings;
	IMotor *m_panMotor;
	IMotor *m_tiltMotor;
};

VideoFrameSender::VideoFrameSender(int port, int width, int height, int codec)
{
	try {
		m_ptr = new SenderPrivate(port, width, height, codec);
	} catch (const std::invalid_argument& ex) {
		std::cout << "catched exception in ctor: " << ex.what() << std::endl;
		throw ex;
	}
}

VideoFrameSender::~VideoFrameSender()
{
	if (m_ptr) {
		delete m_ptr;
		m_ptr = nullptr;
	}
}

void VideoFrameSender::start(DataSupplier ds)
{
	try {
		m_ptr->start(ds);
	} catch (const std::out_of_range& ex) {
		std::cout << "catched exception in start: " << ex.what() << std::endl;
		throw ex;
	}
}

void VideoFrameSender::stop()
{
	m_ptr->stop();
}

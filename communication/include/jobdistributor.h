#ifndef JOB_DISTRIBUTOR_H
#define JOB_DISTRIBUTOR_H

#include <comm_macros.h>
#include <message_handler.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>
#include <atomic>
#include <string>
#include <functional>
#include <vector>

class Data;
class MotorMessageHandler;
class SensorMessageHandler;
struct CommunicationTime;
using Request = std::pair<std::string, MessageType>;
using Response = std::pair<Request, Data*>;
using RequestQueue = tbb::concurrent_bounded_queue<Request>;
using ResponseQueue = tbb::concurrent_bounded_queue<Response>;
using ClientMap = tbb::concurrent_hash_map<std::string, CommunicationTime>;

class COMMUNICATION_EXPORT JobDistributor
{
public:
	JobDistributor(int port, int width, int height, int codec, int delayMicroseconds, int step, const std::vector<int>& panPins, const std::vector<int>& tiltPins);
	~JobDistributor();

	void start();
	void stop();

protected:
	void init();
	void receive();
	void poll(int timeout);
	void remove(const std::string& clientName);
	void send(const std::string& clientName, MessageType messageType, const void* data, int size);

private:
	RequestQueue m_motor_request_queue;
	RequestQueue m_sensor_request_queue;
	RequestQueue m_video_request_queue;
	ResponseQueue m_response_queue;

	MotorMessageHandler *m_motor_executer;
	SensorMessageHandler *m_sensor_executer;
	MessageHandler<RequestQueue, ResponseQueue, MotorMessageHandler> m_motor_message_handler;
	MessageHandler<RequestQueue, ResponseQueue, SensorMessageHandler> m_sensor_message_handler;

	int m_port;
	void* m_context;
	void* m_socket;
	std::atomic<bool> m_run;
	char m_client_id[80];
	std::string m_settings;
	int m_client_id_len;
	std::unique_ptr<tbb::tbb_thread> m_thread;
	ClientMap m_clients;

	std::vector<std::function<void(const std::string&)>> m_functionMap;
};

#endif
#ifndef RVMS_CLIENT_H
#define RVMS_CLIENT_H

#include <string>
#include <connector.h>
#include <tbb/concurrent_queue.h>
#include <data.h>
#include <receive_strategy_queue.h>
#include <decoder.h>
#include <tbb/tbb_thread.h>
#include <memory>
#include <atomic>

class RvmsClient
{
public:
	explicit RvmsClient(const std::string& ip, int port);
	~RvmsClient();

	void start(char *BYTE);
	void stop();
	static void initialize();
	void readSensor();

	void moveRight();
	void moveUp();
	void moveDown();
	void moveLeft();

protected:
	void receive_loop();
	void decode_loop();

private:
	std::atomic<bool> m_started;
	int m_port;
	int m_width, m_height;
	std::string m_ip;
	Connector* m_connector;
	tbb::concurrent_bounded_queue<Data*>* m_frame_queue;
	ReceiveStrategyQueue<tbb::concurrent_bounded_queue<Data*>>* m_receiveStrategy;
	Decoder* m_videoDecoder;
	std::unique_ptr<tbb::tbb_thread> m_receive_thread, m_decode_thread;
	char* m_buffer;
};

#endif // !WEBCAM_CLIENT_H
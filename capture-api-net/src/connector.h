#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>
#include <tbb/concurrent_queue.h>
#include "frame.h"

namespace Client
{
	class Connector
	{
	public:
		explicit Connector(const std::string& url, tbb::concurrent_bounded_queue<spFrame>* frame_queue, int width, int height);
		~Connector();
		
		void start();
		void stop();

	private:
		std::string m_url;
		void* m_context;
		void* m_socket;
		int m_width, m_height;
		tbb::concurrent_bounded_queue<spFrame>* m_frame_queue;
		bool m_started;
		void* m_buffer;
		int m_size;
	};
}

#endif
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>
#include <tbb/concurrent_queue.h>
#include "frame.h"
#include "messages.h"

namespace Client
{
	class Connector
	{
	public:
		explicit Connector(const std::string& url, tbb::concurrent_bounded_queue<spFrame>* frame_queue);
		~Connector();
		
		void start();
		void stop();
		inline int getWidth() { return m_width; }
		inline int getHeight() { return m_height; }
		inline int getCodec() { return m_codec; }

	protected:
		void init();
		void sendRequest(MessageType message);

	private:
		std::string m_url;
		void* m_context;
		void* m_socket;
		int m_width, m_height, m_codec;
		tbb::concurrent_bounded_queue<spFrame>* m_frame_queue;
		bool m_started;
		int m_size;
	};
}

#endif
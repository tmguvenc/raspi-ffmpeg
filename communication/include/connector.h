#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>
#include <tbb/concurrent_queue.h>
#include <comm_macros.h>

class VideoFrame;
class ConnectorPrivate;
using spVideoFrame = std::shared_ptr<VideoFrame>;

class COMMUNICATION_EXPORT Connector
{
public:
	explicit Connector(const std::string& url, tbb::concurrent_bounded_queue<spVideoFrame>* frame_queue);
	~Connector();

	void start();
	void stop();
	inline int getWidth();
	inline int getHeight();
	inline int getCodec();

private:
	ConnectorPrivate* m_ptr;
};

#endif
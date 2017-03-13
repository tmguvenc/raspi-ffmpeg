#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string>
#include <comm_macros.h>

class ConnectorPrivate;
class IReceiveStrategy;

class COMMUNICATION_EXPORT Connector
{
public:
	explicit Connector(const std::string& url, IReceiveStrategy* strategy);
	~Connector();

	void start();
	void stop();
	void readSensor();
	inline int getWidth();
	inline int getHeight();
	inline int getCodec();

private:
	ConnectorPrivate* m_ptr;
};

#endif
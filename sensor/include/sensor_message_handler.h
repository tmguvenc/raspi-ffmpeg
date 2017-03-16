#ifndef SENSOR_MESSAGE_HANDLER_H
#define SENSOR_MESSAGE_HANDLER_H

#include <message_handler.h>
#include <hum_temp_sensor_data.h>
#include <hum_temp_sensor.h>
#include <string>
#include <sensor_data.h>
#include <messages.h>

template<>
class Handler<HumTempSensorData, MessageType>
{
public:
	std::unique_ptr<Data> execute(const MessageType&)
	{
		auto data = sensor.readData();
		auto h = data->getData(Humidity);
		auto t = data->getData(Temperature);
		auto aa = std::to_string(h) + ";" + std::to_string(t);
		return std::make_unique<HumTempSensorData>(aa.c_str(), aa.size());
	}
private:
	HumidityTemperatureSensor sensor;
};

using SensorMessageHandler = Handler<HumTempSensorData, MessageType>;

#endif
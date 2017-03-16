#ifndef SENSOR_MESSAGE_HANDLER_H
#define SENSOR_MESSAGE_HANDLER_H

#include <message_handler.h>
#include <hum_temp_sensor_data.h>
#include <hum_temp_sensor.h>
#include <string>
#include <sensor_data.h>
#include <messages.h>

class SensorMessageHandler
{
public:
	Data* execute(const MessageType&)
	{
		auto data = sensor.readData();
		auto h = data->getData(Humidity);
		auto t = data->getData(Temperature);
		auto aa = std::to_string(h) + ";" + std::to_string(t);
		return new HumTempSensorData(aa.c_str(), aa.size());
	}
private:
	HumidityTemperatureSensor sensor;
};

#endif
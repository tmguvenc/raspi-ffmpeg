#ifndef HUMIDITY_TEMP_SENSOR_H
#define HUMIDITY_TEMP_SENSOR_H

#include <sensor_base.h>

class SENSOR_EXPORT HumidityTemperatureSensor : ISensor
{
public:
	HumidityTemperatureSensor();
	virtual std::unique_ptr<ISensorData> readData() override;
};

#endif

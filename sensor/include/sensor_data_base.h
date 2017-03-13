#ifndef SENSOR_DATA_BASE_H
#define SENSOR_DATA_BASE_H

#include <limits>

enum SensorDataType
{
	Temperature,
	Humidity,
	Pressure
};

class ISensorData
{
public:
	virtual ~ISensorData() { }
	virtual float getData(SensorDataType){
		return std::numeric_limits<float>::min();
	}
};

#endif
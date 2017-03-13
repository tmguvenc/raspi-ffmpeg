#ifndef SENSOR_DATA_BASE_H
#define SENSOR_DATA_BASE_H

#include <limits>
#include <sensor_macros.h>

enum SensorDataType
{
	Temperature,
	Humidity,
	Pressure
};

#undef min

class SENSOR_EXPORT ISensorData
{
public:
	virtual ~ISensorData() { }
	virtual float getData(SensorDataType){
		return std::numeric_limits<float>::min();
	}
};

#endif

#ifndef SENSOR_H
#define SENSOR_H

#include <sensor_macros.h>
#include <memory>

class ISensorData;

class ISensor
{
public:
	virtual ~ISensor() { }
	virtual std::unique_ptr<ISensorData> readData() = 0;
};

#endif
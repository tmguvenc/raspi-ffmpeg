#ifndef HUM_TEMP_SENSOR_DATA_H
#define HUM_TEMP_SENSOR_DATA_H

#include <sensor_data_base.h>

class HumidityTemperatureSensorData : public ISensorData
{
public:
	HumidityTemperatureSensorData(float hum, float temp) : m_hum(hum), m_temp(temp) { 

	}

	virtual float getData(SensorDataType dataType) override{
		if (dataType == Humidity)
			return m_hum;
		if (dataType == Temperature)
			return m_temp;

		return ISensorData::getData(dataType);
	}

private:
	float m_hum, m_temp;
};

#endif
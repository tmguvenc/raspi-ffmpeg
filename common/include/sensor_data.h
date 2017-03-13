#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <data.h>
#include <cstring>
#include <common_utils.h>
#include <string>

class HumTempSensorData : public Data
{
public:
	explicit HumTempSensorData(uint32_t size) : Data(size)
	{
		static_assert(std::is_base_of<Data, HumTempSensorData>::value,
			"Sensor Data must be derived from Data.");
	}
	HumTempSensorData(void* data, uint32_t size) : Data(size) {
		memcpy(m_data, data, size);
		
		auto sensorData = std::move(split(std::string(static_cast<char*>(m_data), size), ';'));

		m_h = std::atof(sensorData[0].c_str());
		m_t = std::atof(sensorData[1].c_str());
	}
	
	int type() override{
		return 2;
	}

	inline float getHumidity() const {
		return m_h;
	}
	
	inline float getTemperature() const {
		return m_t;
	}
	
private:
	float m_h, m_t;
};

#endif

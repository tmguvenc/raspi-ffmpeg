#include <sensor_data.h>

HumTempSensorData::HumTempSensorData(uint32_t size) : Data(size)
{
	static_assert(std::is_base_of<Data, HumTempSensorData>::value,
		"Sensor Data must be derived from Data.");
}

HumTempSensorData::HumTempSensorData(void* data, uint32_t size): Data(size)
{
	memcpy(m_data, data, size);
		
	auto sensorData = std::move(split(std::string(static_cast<char*>(m_data), size), ';'));

	m_h = std::atof(sensorData[0].c_str());
	m_t = std::atof(sensorData[1].c_str());
}

int HumTempSensorData::type()
{
	return 2;
}

float HumTempSensorData::getHumidity() const
{
	return m_h;
}

float HumTempSensorData::getTemperature() const
{
	return m_t;
}
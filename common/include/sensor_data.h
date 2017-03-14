#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <data.h>
#include <cstring>
#include <common_utils.h>
#include <string>
#include <common_macros.h>

class COMMON_EXPORT HumTempSensorData : public Data
{
public:
	explicit HumTempSensorData(uint32_t size);
	HumTempSensorData(void* data, uint32_t size);
	int type() override;
	inline float getHumidity() const;
	inline float getTemperature() const;

private:
	float m_h, m_t;
};

#endif

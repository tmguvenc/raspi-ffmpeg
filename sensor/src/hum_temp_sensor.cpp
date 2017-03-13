#include <hum_temp_sensor.h>
#include <hum_temp_sensor_data.h>

HumidityTemperatureSensor::HumidityTemperatureSensor(){

}

std::unique_ptr<ISensorData> HumidityTemperatureSensor::readData(){
	// TODO:
	return std::make_unique<HumidityTemperatureSensorData>(-1, -1);
}
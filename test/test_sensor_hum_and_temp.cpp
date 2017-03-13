#include <gtest/gtest.h>
#include <hum_temp_sensor.h>
#include <limits>
#include <sensor_data_base.h>
#include <wiringPi.h>

struct SensorState
{
	bool valid;
};

struct ArgumentParserTestAll : testing::TestWithParam<SensorState>
{
	HumidityTemperatureSensor* sensor;
	ArgumentParserTestAll()
	{
		wiringPiSetup();
		sensor = new HumidityTemperatureSensor;
	}
	virtual ~ArgumentParserTestAll()
	{
		delete sensor;
	}
};

TEST_F(ArgumentParserTestAll, Sensor){
	auto data = sensor->readData();

	auto h = data->getData(Humidity);
	auto t = data->getData(Temperature);
	printf("Humidity: %f\n", h);
	printf("Temperature: %f\n", t);

	EXPECT_NE(h, std::numeric_limits<float>::min());
	EXPECT_NE(t, std::numeric_limits<float>::min());
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

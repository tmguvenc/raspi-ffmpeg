#include <gtest/gtest.h>
#include <hum_temp_sensor.h>
#include <limits>
#include <sensor_data_base.h>

#ifdef __linux__
#include <wiringPi.h>
#endif

struct SensorState
{
	bool valid;
};

struct ArgumentParserTestAll : testing::TestWithParam<SensorState>
{
	HumidityTemperatureSensor* sensor;
	ArgumentParserTestAll()
	{
#ifdef __linux__
		wiringPiSetup();
#endif
		sensor = new HumidityTemperatureSensor;
	}
	virtual ~ArgumentParserTestAll()
	{
		delete sensor;
	}
};

TEST_F(ArgumentParserTestAll, Sensor){
	auto data = sensor->readData();

#ifdef __linux__
	EXPECT_NE(data->getData(Humidity), std::numeric_limits<float>::min());
	EXPECT_NE(data->getData(Temperature), std::numeric_limits<float>::min());
#else
	EXPECT_EQ(data->getData(Humidity), std::numeric_limits<float>::min());
	EXPECT_EQ(data->getData(Temperature), std::numeric_limits<float>::min());
#endif
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

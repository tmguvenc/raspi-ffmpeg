
#include <hum_temp_sensor.h>
#include <hum_temp_sensor_data.h>
#include <limits>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <rgpio.h>

static const auto min_f = std::numeric_limits<float>::min();

HumidityTemperatureSensor::HumidityTemperatureSensor(){}

#define DHTPIN 7
#define MAXTIMINGS 85

static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read)
{
	/* digitalRead() and friends from wiringpi are defined as returning a value
	< 256. However, they are returned as int() types. This is a safety function */
	if (read > 255 || read < 0){
    		printf("Invalid data from wiringPi library\n");
    		exit(EXIT_FAILURE);
  	}
	return (uint8_t)read;
}

inline bool readDHT22(float& h, float& t){
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

	// pull pin down for 18 milliseconds
	GPIO::setPinMode(DHTPIN, OUTPUT);
	GPIO::write(DHTPIN, HIGH);
	GPIO::sleepMilliSeconds(10);
	GPIO::write(DHTPIN, LOW);
	GPIO::sleepMilliSeconds(18);
	// then pull it up for 40 microseconds
	GPIO::write(DHTPIN, HIGH);
	GPIO::sleepMicroSeconds(40);
	// prepare to read the pin
	GPIO::setPinMode(DHTPIN, INPUT);

	// detect change and read data
	for (i = 0; i < MAXTIMINGS; i++) {
		counter = 0;
		while (sizecvt(GPIO::read(DHTPIN)) == laststate) {
			counter++;
			GPIO::sleepMicroSeconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = sizecvt(GPIO::read(DHTPIN));

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i % 2 == 0)) {
			// shove each bit into the storage bytes
			dht22_dat[j / 8] <<= 1;
			if (counter > 16)
				dht22_dat[j / 8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) &&
		(dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF))) {
		h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
		h /= 10;
		t = (float)(dht22_dat[2] & 0x7F) * 256 + (float)dht22_dat[3];
		t /= 10.0;
		if ((dht22_dat[2] & 0x80) != 0)  t *= -1;

		return true;
	}

	return false;
}


std::unique_ptr<ISensorData> HumidityTemperatureSensor::readData(){
	float h = min_f, t = min_f;
	int counter = 100;
	while(!readDHT22(h, t) && --counter)
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	return std::make_unique<HumidityTemperatureSensorData>(h, t);
}
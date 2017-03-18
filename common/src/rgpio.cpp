#include <rgpio.h>

void GPIO::setup()
{
#ifdef __linux__
	wiringPiSetup();
#endif
	return;
}

void GPIO::write(uint8_t pin, uint8_t value)
{
#ifdef __linux__
	digitalWrite(pin, value);
#endif
	return;
}

int GPIO::read(uint8_t pin)
{
#ifdef __linux__
	return digitalRead(pin);
#else
	return -1;
#endif
}

void GPIO::sleepMilliSeconds(uint32_t milliSeconds)
{
#ifdef __linux__
	delay(milliSeconds);
#endif
	return;
}

void GPIO::sleepMicroSeconds(uint32_t microSeconds)
{
#ifdef __linux__
	delayMicroseconds(microSeconds);
#endif
	return;
}

void GPIO::setPinMode(uint8_t pin, uint8_t mode)
{
#ifdef __linux__
	pinMode(pin, mode);
#endif
	return;
}
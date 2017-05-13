#include <rgpio.h>

void GPIO::setup()
{
#ifdef WIRINGPI
	wiringPiSetup();
#endif
	return;
}

void GPIO::write(uint8_t pin, uint8_t value)
{
#ifdef WIRINGPI
	digitalWrite(pin, value);
#endif
	return;
}

int GPIO::read(uint8_t pin)
{
#ifdef WIRINGPI
	return digitalRead(pin);
#else
	return -1;
#endif
}

void GPIO::sleepMilliSeconds(uint32_t milliSeconds)
{
#ifdef WIRINGPI
	delay(milliSeconds);
#endif
	return;
}

void GPIO::sleepMicroSeconds(uint32_t microSeconds)
{
#ifdef WIRINGPI
	delayMicroseconds(microSeconds);
#endif
	return;
}

void GPIO::setPinMode(uint8_t pin, uint8_t mode)
{
#ifdef WIRINGPI
	pinMode(pin, mode);
#endif
	return;
}
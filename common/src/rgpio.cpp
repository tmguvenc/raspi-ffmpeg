#include <rgpio.h>

void GPIO::setup()
{
#ifdef __linux__
	wiringPiSetup();
#endif
	return;
}

void GPIO::write(int pin, int value)
{
#ifdef __linux__
	digitalWrite(pin, value);
#endif
	return;
}

int GPIO::read(int pin)
{
#ifdef __linux__
	return digitalRead(pin);
#else
	return -1;
#endif
}

void GPIO::sleepMilliSeconds(int milliSeconds)
{
#ifdef __linux__
	delay(milliSeconds);
#endif
	return;
}

void GPIO::sleepMicroSeconds(int microSeconds)
{
#ifdef __linux__
	delayMicroseconds(microSeconds);
#endif
	return;
}

void GPIO::setPinMode(int pin, int mode)
{
#ifdef __linux__
	pinMode(pin, mode);
#endif
	return;
}
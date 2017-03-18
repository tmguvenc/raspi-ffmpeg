#ifndef RGPIO_H
#define RGPIO_H

#include <common_macros.h>

#ifdef __linux__
#include <wiringPi.h>
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

#ifndef INPUT
#define INPUT 0
#endif
#include <stdint.h>

class COMMON_EXPORT GPIO
{
public:
	static void setup();
	static void write(uint8_t pin, uint8_t value);
	static int read(uint8_t pin);
	static void sleepMilliSeconds(uint32_t milliSeconds);
	static void sleepMicroSeconds(uint32_t microSeconds);
	static void setPinMode(uint8_t pin, uint8_t mode);
};

#endif
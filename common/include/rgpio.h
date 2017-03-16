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

class COMMON_EXPORT GPIO
{
public:
	static void setup();
	static void write(int pin, int value);
	static int read(int pin);
	static void sleepMilliSeconds(int milliSeconds);
	static void sleepMicroSeconds(int microSeconds);
	static void setPinMode(int pin, int mode);
};

#endif
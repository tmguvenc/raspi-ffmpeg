#ifndef SENSOR_MACROS_H
#define SENSOR_MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef SENSOR_EXPORTS
		#define SENSOR_EXPORT __declspec(dllexport)
	#else
		#define SENSOR_EXPORT __declspec(dllimport)
	#endif
#else
	#define SENSOR_EXPORT
#endif

#endif
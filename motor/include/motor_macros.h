#ifndef MOTOR_MACROS_H
#define MOTOR_MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef MOTOR_EXPORTS
		#define MOTOR_EXPORT __declspec(dllexport)
	#else
		#define MOTOR_EXPORT __declspec(dllimport)
	#endif
#else
	#define MOTOR_EXPORT
#endif

#endif
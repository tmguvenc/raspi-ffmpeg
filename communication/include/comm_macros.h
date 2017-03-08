#ifndef COMMUNICATION_MACROS_H
#define COMMUNICATION_MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef COMMUNICATION_EXPORTS
		#define COMMUNICATION_EXPORT __declspec(dllexport)
	#else
		#define COMMUNICATION_EXPORT __declspec(dllimport)
	#endif
#else
	#define COMMUNICATION_EXPORT
#endif

#endif
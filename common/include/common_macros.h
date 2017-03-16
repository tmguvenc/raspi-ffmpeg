#ifndef COMMON_MACROS_H
#define COMMON_MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef COMMON_EXPORTS
		#define COMMON_EXPORT __declspec(dllexport)
	#else
		#define COMMON_EXPORT __declspec(dllimport)
	#endif
#else
	#define COMMON_EXPORT
#endif

#endif
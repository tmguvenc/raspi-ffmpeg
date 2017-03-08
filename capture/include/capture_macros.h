#ifndef CAPTURE_MACROS_H
#define CAPTURE_MACROS_H

#if defined(_WIN32) && defined(_MSC_VER)
	#ifdef CAPTURE_EXPORTS
		#define CAPTURE_EXPORT __declspec(dllexport)
	#else
		#define CAPTURE_EXPORT __declspec(dllimport)
	#endif
#else
	#define CAPTURE_EXPORT
#endif

#endif

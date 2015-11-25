#ifndef __brf_PreProcessorConstants__
#define __brf_PreProcessorConstants__

/**
 * Some pre defined settings regarding threading.
 */
#ifdef _EMSCRIPTEN
    #define THREADS_ENABLED 0
    #define NUM_THREADS 1
#elif _WIN64
	#define THREADS_ENABLED 1
	#define NUM_THREADS 8
#elif _WIN32
	#define THREADS_ENABLED 1
	#define NUM_THREADS 8
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
		#define THREADS_ENABLED 0
		#define NUM_THREADS 1
    #elif TARGET_OS_IPHONE
		#define THREADS_ENABLED 1
		#define NUM_THREADS 2
    #elif TARGET_OS_MAC
		#define THREADS_ENABLED 1
		#define NUM_THREADS 8
    #else
		#define THREADS_ENABLED 0
		#define NUM_THREADS 1
    #endif
#elif __ANDROID__
	#define THREADS_ENABLED 1
	#define NUM_THREADS 2
#elif __linux
	#define THREADS_ENABLED 1
	#define NUM_THREADS 8
#elif __unix // all unices not caught above
	#define THREADS_ENABLED 0
	#define NUM_THREADS 1
#elif __posix
	#define THREADS_ENABLED 0
	#define NUM_THREADS 1
#else
	#define THREADS_ENABLED 0
	#define NUM_THREADS 1
#endif

#endif /* __brf_PreProcessorConstants__ */

#ifndef AXON_UTILITY_H
#define AXON_UTILITY_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WINDOWS_PLATFORM
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#define UNIX_PLATFORM
#endif



#if defined(WINDOWS_PLATFORM)
#if defined(AXON_LIB)
#define AXON_DECLSPEC __declspec(dllexport)
#else
#define AXON_DECLSPEC __declspec(dllimport)
#endif

#elif defined(UNIX_PLATFORM)
#define AXON_DECLSPEC __attribute__((visibility("default")))
#else
#define AXON_DECLSPEC
#endif

#endif

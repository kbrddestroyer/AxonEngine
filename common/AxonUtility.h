#ifndef AXON_UTILITY_H
#define AXON_UTILITY_H

#if defined(WIN32)
#if defined(AXON_LIB)
#define AXON_DECLSPEC __declspec(dllexport)
#else
#define AXON_DECLSPEC __declspec(dllimport)
#endif

#elif defined(__unix__)
#define AXON_DECLSPEC
#else
#define AXON_DECLSPEC
#endif

#endif
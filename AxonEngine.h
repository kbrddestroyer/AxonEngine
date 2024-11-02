#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define WINDOWS_PLATFORM
#endif
#if __APPLE__ || __APPLE_CC__ || defined(__unix__)
#define UNIX_PLATFORM
#endif

#include <AxonError.h>
#include <AxonTypes.h>

#if defined(WINDOWS_PLATFORM)
#define AXON_EXPORT __declspec(dllexport)
#elif defined(UNIX_PLATFORM)
#define AXON_EXPORT
#else
#define AXON_EXPORT
#endif
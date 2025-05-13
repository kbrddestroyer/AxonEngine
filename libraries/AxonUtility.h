#ifndef AXON_UTILITY_H
#define AXON_UTILITY_H

#include <common_macro.h>

#if defined(WINDOWS_PLATFORM)
#if defined(AXON_LIB)
#define AXON_DECLSPEC __declspec(dllexport)
#else
#define AXON_DECLSPEC __declspec(dllimport)
#endif
#elif defined(UNIX_PLATFORM)
#if defined(__clang__) || defined(__GNUC__)
#define AXON_DECLSPEC __attribute__((visibility("default")))
#else
#define AXON_DECLSPEC
#endif
#endif
#endif

/*
*	Global AxonEngine entrypoint both for client and server
*	Includes all required libraries and defines some platform-specific macro
*	WINDOWS_PLATFORM - win32/64 target
*	UNIX_PLATFORM - both unix and macOS devices
*/
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define WINDOWS_PLATFORM
#endif
#if __APPLE__ || __APPLE_CC__ || defined(__unix__)
#define UNIX_PLATFORM
#endif

#if defined(WINDOWS_PLATFORM)
#if defined(AXON_SERVER_LIB)
#define AXON_EXPORT __declspec(dllexport)
#else
#define AXON_EXPORT __declspec(dllimport)
#endif
#elif defined(UNIX_PLATFORM)
#define AXON_EXPORT
#else
#define AXON_EXPORT
#endif

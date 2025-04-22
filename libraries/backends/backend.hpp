#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#if defined(USE_BERKELEY)
#include "berkeley/basic_networking.h"
#else
	/* DEFAULT */
#endif

#include "berkeley/basic_networking.h"
#ifdef __cplusplus
}
#endif

template <uint8_t> int32_t send_message(const Socket& socket, const char* message, size_t size);

template <uint8_t> int32_t recv_message(Socket& socket, char* buffer, size_t size_allocated);

template <uint8_t> uint8_t initialize_server(Socket& socket, uint32_t port);

template <uint8_t> uint8_t initialize_client(Socket& socket, const char* hostname, uint32_t port);

template <uint8_t> void finalize(Socket& socket);

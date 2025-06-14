/**
 * backend library.
 * Contains functions for protocol-independent socket functions usage. Works with Berkeley sockets.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "berkeley/basic_networking.h"
#include "custom_utility.h"
#ifdef __cplusplus
}
#endif

template <uint8_t> int32_t send_message(const Socket& socket, const void* message, size_t size) = delete;
template <uint8_t> int32_t recv_message(Socket& socket, void* buffer, size_t size_allocated) = delete;
template <uint8_t> uint8_t initialize_server(Socket& socket, uint32_t port) = delete;
template <uint8_t> uint8_t initialize_client(Socket& socket, const char* hostname, uint32_t port) = delete;
template <uint8_t> void finalize(Socket& socket);

template <> int32_t send_message<SOCK_STREAM>(const Socket&, const void*, size_t);
template <> int32_t send_message<SOCK_DGRAM>(const Socket&, const void*, size_t);

template <> int32_t recv_message<SOCK_STREAM>(Socket&, void*, size_t);
template <> int32_t recv_message<SOCK_DGRAM>(Socket&, void*, size_t);

template <> uint8_t initialize_server<SOCK_STREAM>(Socket&, uint32_t);
template <> uint8_t initialize_server<SOCK_DGRAM>(Socket&, uint32_t);

template <> uint8_t initialize_client<SOCK_STREAM>(Socket&, const char*, uint32_t);
template <> uint8_t initialize_client<SOCK_DGRAM>(Socket&, const char*, uint32_t);

template <> void finalize<SOCK_STREAM>(Socket &);
template <> void finalize<SOCK_DGRAM>(Socket &);

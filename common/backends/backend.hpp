#ifndef BACKEND_H
#define BACKEND_H

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


template <uint8_t> int32_t send_message(const Socket socket, const char* message, size_t size) = delete;

template <> int32_t send_message<SOCK_STREAM>(const Socket socket, const char* message, size_t size)
{
	return send_tcp_message(message, size, socket.socket);
}

template <> int32_t send_message<SOCK_DGRAM>(const Socket socket, const char* message, size_t size)
{
	return send_udp_message(message, size, socket.socket, &socket.conn);
}


template <uint8_t> int32_t recv_message(Socket socket, char* const buffer, size_t size_allocated) = delete;

template <> int32_t recv_message<SOCK_STREAM>(Socket socket, char* const buffer, size_t size_allocated)
{
	return recv_tcp_message(buffer, size, socket.socket);
}

template <> int32_t recv_message<SOCK_DGRAM>(Socket socket, char* const buffer, size_t size_allocated)
{
	return recv_udp_message(buffer, size, socket.socket, &socket.conn);
}


template <uint8_t> uint8_t initialize_server(Socket& socket, uint32_t port) = delete;

template <> uint8_t initialize_server<SOCK_STREAM>(Socket& socket, uint32_t port)
{
	return create_tcp_server(&socket.conn, &socket.socket, port);
}

template <> uint8_t initialize_server<SOCK_DGRAM>(Socket& socket, uint32_t port)
{
	return create_udp_server(&socket.conn, &socket.socket, port);
}


template <uint8_t> uint8_t initialize_client(Socket& socket, const char* hostname, uint32_t port) = delete;

template <> uint8_t initialize_client<SOCK_STREAM>(Socket& socket, const char* hostname, uint32_t port)
{
	connect_tcp_client(&socket.conn, &socket.socket, hostname, port);
}

template <> uint8_t initialize_client<SOCK_DGRAM>(Socket& socket, const char* hostname, uint32_t port)
{
	connect_udp_client(&socket.conn, &socket.socket, hostname, port);
}

#endif

#include "backend.hpp"

template <> int32_t send_message<SOCK_STREAM>(const Socket& socket, const void* message, size_t size)
{
    return send_tcp_message(message, size, socket.socket);
}

template <> int32_t send_message<SOCK_DGRAM>(const Socket& socket, const void* message, size_t size)
{
    return send_udp_message(message, size, socket.socket, &socket.conn);
}

template <> int32_t recv_message<SOCK_STREAM>(Socket& socket, void* const buffer, size_t size_allocated)
{
    return recv_tcp_message(buffer, size_allocated, socket.socket);
}

template <> int32_t recv_message<SOCK_DGRAM>(Socket& socket, void* const buffer, size_t size_allocated)
{
    return recv_udp_message(buffer, size_allocated, socket.socket, &socket.conn);
}

template <> uint8_t initialize_server<SOCK_STREAM>(Socket& socket, uint32_t port)
{
    return create_tcp_server(&socket.conn, &socket.socket, port);
}

template <> uint8_t initialize_server<SOCK_DGRAM>(Socket& socket, uint32_t port)
{
    return create_udp_server(&socket.conn, &socket.socket, port);
}

template <> uint8_t initialize_client<SOCK_STREAM>(Socket& socket, const char* hostname, const uint32_t port)
{
    return connect_tcp_client(&socket.conn, &socket.socket, hostname, port);
}

template <> uint8_t initialize_client<SOCK_DGRAM>(Socket& socket, const char* hostname, const uint32_t port)
{
    return connect_udp_client(&socket.conn, &socket.socket, hostname, port);
}

template <> void finalize<SOCK_STREAM> (Socket& socket)
{
    finalize_tcp(socket.socket);
}

template <> void finalize<SOCK_DGRAM> (Socket& socket)
{
    finalize_udp(socket.socket);
}
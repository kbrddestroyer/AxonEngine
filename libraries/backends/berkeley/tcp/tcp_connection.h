#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <stdint.h>
#include <backends/berkeley/master_include.h>

#pragma region TCP_CONNECTION
uint8_t connect_tcp_client(SOCKADDR_IN_T *, SOCKET_T *, const char *, uint32_t);
uint8_t create_tcp_server(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);
int32_t send_tcp_message(const void*, size_t, SOCKET_T);
int32_t recv_tcp_message(void*, size_t, SOCKET_T);
SOCKET_T accept_incoming(SOCKET_T, SOCKADDR_IN_T*);
void finalize_tcp(SOCKET_T);
#pragma endregion

#endif

#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include <stdint.h>
#include <backends/berkeley/master_include.h>

uint8_t connect_udp_client(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);
uint8_t create_udp_server(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);
int32_t send_udp_message(const void*, size_t, SOCKET_T, const SOCKADDR_IN_T*);
int32_t recv_udp_message(void*, size_t, SOCKET_T, SOCKADDR_IN_T*);
void finalize_udp(SOCKET_T);

#endif

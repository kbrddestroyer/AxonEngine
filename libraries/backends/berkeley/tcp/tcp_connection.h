#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <stdint.h>
#include <backends/berkeley/master_include.h>

#pragma region TCP_CONNECTION

/**
* Connects to the remote host via TCP. Uses getaddrinfo.
* @param server
* @param client
* @param char* hostname
* @param port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t connect_tcp_client(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);

/**
* Initializes server socket with TCP protocol.
* @param server
* @param server_socket
* @param port
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
uint8_t create_tcp_server(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);

/**
* Sends data over UDP
* @param char* message - data to send, sequence of bytes
* @param size - size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param from - source
* @param to - destination
* @returns sendto result
*/
int32_t send_tcp_message(const void*, size_t, SOCKET_T);

/**
* Handles message receiving over UDP
* @param buffer - data buffer
* @param max_size - max size of bytes that can be written in buffer
* @returns recvfrom result
*/
int32_t recv_tcp_message(void* const, size_t, SOCKET_T);

SOCKET_T accept_incoming(SOCKET_T, SOCKADDR_IN_T*);

/**
* Closes socket
* @param socket
*/
void finalize_tcp(SOCKET_T);

#pragma endregion

#endif

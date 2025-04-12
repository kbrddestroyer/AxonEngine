#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <stdint.h>
#include "backends/berkeley/master_include.h"

#pragma region TCP_CONNECTION

/**
* Connects to the remote host via TCP. Uses getaddrinfo.
* @param SOCKADDR_IN_T* server
* @param SOCKET* client
* @param const char* hostname
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t connect_tcp_client(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);

/**
* Initializes server socket with TCP protocol.
* @param SOCKADDR_IN_T* server
* @param SOCKET* server_socket
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t create_tcp_server(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);



/**
* Sends data over UDP
* @param const char* message - data to send, sequence of bytes
* @param size_t size - size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param SOCKET_T from - source
* @param SOCKADDR_IN_T to - destination
* @returns sendto result
*/
int32_t send_tcp_message(const char*, size_t, SOCKET_T);

/**
* Handles message receiving over UDP
* @param char** buffer - data buffer
* @param size_t max_size - max size of bytes that can be written in buffer
* @returns recvfrom result
*/
int32_t recv_tcp_message(char* const, size_t, SOCKET_T);

SOCKET_T accept_incoming(SOCKET_T, SOCKADDR_IN_T*);

/**
* Closes socket
* @param SOCKET_T* socket
*/
void finalize_tcp(SOCKET_T);

#pragma endregion

#endif

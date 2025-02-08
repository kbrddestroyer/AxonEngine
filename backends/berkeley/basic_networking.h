/*
* Can be used as template for any other backends
* 
* Defines some low-level socket logic. Direct usage is not recommended
* Created: 06.02.25
*/

#ifndef BASIC_NETWORKING_H
#define BASIC_NETWORKING_H

#include "master_include.h"
#include <stdint.h>

#pragma region CLIENT_UTILS

/**
* Initializes client connection to remote host. Uses getaddrinfo.
* @param SOCKADDR_IN_T* server
* @param SOCKET* client
* @param const char* hostname
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t initializeClientConnection(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);

#pragma endregion /* CLIENT UTILITY FUNCTIONS */

#pragma region SERVER_UTILS

/**
* Initializes server socket.
* @param SOCKADDR_IN_T* server
* @param SOCKET* server_socket
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t initializeServerSocket(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);

#pragma endregion /* SERVER UTILITY FUNCTIONS */

int32_t send_message(const char*, size_t, SOCKET_T*, SOCKADDR_IN_T*);
int32_t recv_message(char**, size_t, SOCKET_T, SOCKADDR_IN*);

void finalize(SOCKET_T*);

#endif
/* basic_networking.h */
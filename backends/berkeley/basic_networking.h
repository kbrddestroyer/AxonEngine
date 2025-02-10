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

#pragma region UDP_UTILS
#pragma region CLIENT_UTILS

/**
* Initializes client connection to remote host. Uses getaddrinfo.
* @param SOCKADDR_IN_T* server
* @param SOCKET* client
* @param const char* hostname
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint32_t initializeClientConnection(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);

#pragma endregion /* CLIENT UTILITY FUNCTIONS */

#pragma region SERVER_UTILS

/**
* Initializes server socket.
* @param SOCKADDR_IN_T* server
* @param SOCKET* server_socket
* @param uint32_t port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint32_t initializeServerSocket(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);

#pragma endregion /* SERVER UTILITY FUNCTIONS */

/**
* Sends data over UDP
* @param const char* message - data to send, sequence of bytes
* @param size_t size - size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param SOCKET_T from - source
* @param SOCKADDR_IN_T to - destination
* @returns sendto result
*/
int32_t send_message(const char*, size_t, SOCKET_T, SOCKADDR_IN_T*);

/**
* Handles message receiving over UDP
* @param char** buffer - data buffer
* @param size_t max_size - max size of bytes that can be written in buffer
*/
int32_t recv_message(char**, size_t, SOCKET_T, SOCKADDR_IN*);

/**
* Closes socket
* @param SOCKET_T* socket
*/
void finalize(SOCKET_T*);

#pragma endregion /* UDP UTILS */

#pragma region TCP_UTILS

#pragma endregion /* TCP UTILS */
#endif


/* basic_networking.h */

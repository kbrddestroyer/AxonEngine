#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include <stdint.h>
#include <backends/berkeley/master_include.h>

#pragma region UDP_UTILS
#pragma region CLIENT_UTILS

/**
* Connects to the remote host via UDP. Uses getaddrinfo.
* @param server
* @param client
* @param hostname
* @param port
* @return 0 or ERR_CODE (defined in basic_networking.h)
*/
uint8_t connect_udp_client(SOCKADDR_IN_T*, SOCKET_T*, const char*, uint32_t);

#pragma endregion /* CLIENT UTILITY FUNCTIONS */

#pragma region SERVER_UTILS

/**
* Initializes server socket with UDP protocol.
* @param server
* @param server_socket
* @param port
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
uint8_t create_udp_server(SOCKADDR_IN_T*, SOCKET_T*, uint32_t);

#pragma endregion /* SERVER UTILITY FUNCTIONS */

/**
* Sends data over UDP
* @param message - data to send, sequence of bytes
* @param size - size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param from - source
* @param to - destination
* @return sendto result
*/
int32_t send_udp_message(const void*, size_t, SOCKET_T, const SOCKADDR_IN_T*);

/**
* Handles message receiving over UDP
* @param buffer - data buffer
* @param max_size - max size of bytes that can be written in buffer
* @return recvfrom result
*/
int32_t recv_udp_message(void*, size_t, SOCKET_T, SOCKADDR_IN_T*);

/**
* Closes socket
* @param socket
*/
void finalize_udp(SOCKET_T);

#pragma endregion /* UDP UTILS */
#endif

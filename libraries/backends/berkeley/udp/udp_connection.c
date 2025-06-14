#include "udp_connection.h"

#include <stdio.h>
#include <string.h>

#pragma region UDP_UTILS

/**
* Connects to the remote host via UDP. Uses `getaddrinfo`.
* @param [out] server server info (SOCKADDR_IN_T structure)
* @param client client socket
* @param hostname ip/hostname of a node to connect to
* @param port port of a node to connect to
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
uint8_t connect_udp_client(SOCKADDR_IN_T* server, SOCKET_T* client, const char* hostname, uint32_t port)
{
	SOCKET_HEAD_INIT

		if (!CHECK_VALID(*client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
			return ERR_INVALID;

	ADDRINFO_T hints = {0};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	ADDRINFO_T* res;

	char port_s[16] = { 0 };
	snprintf(port_s, sizeof(port_s), "%d", port);

	if (getaddrinfo(hostname, port_s, &hints, &res) != 0)
	{
		return ERR_GETADDRINFO_FAIL;
	}

	memcpy(server, (SOCKADDR_IN_T*)res->ai_addr, res->ai_addrlen);

	SET_ASYNC_SOCKET(10, *client);

	freeaddrinfo(res);
	return SUCCESS;
}

/**
* Initializes server socket with UDP protocol.
* @param [out] server server info (SOCKADDR_IN_T structure)
* @param [out] server_socket server socket that will be initialized
* @param port port to create server onto
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
uint8_t create_udp_server(SOCKADDR_IN_T* server, SOCKET_T* server_socket, uint32_t port)
{
	SOCKET_HEAD_INIT

		if (!CHECK_VALID(*server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
			return ERR_INVALID;
		}

	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	server->sin_addr.s_addr = INADDR_ANY;

	if (!CHECK_VALID(bind(*server_socket, (SOCKADDR_T*)server, sizeof(*server)))) {
		return ERR_COULD_NOT_BIND;
	}
	SET_ASYNC_SOCKET(10, *server_socket);
	return SUCCESS;
}

/**
* Sends data over UDP
* @param message data to send, sequence of bytes
* @param size size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param from source
* @param to destination
* @return sendto function result
*/
int32_t send_udp_message(const void* message, const size_t size, const SOCKET_T from, const SOCKADDR_IN_T* to)
{
	return sendto(from, message, size, 0, (SOCKADDR_T*)to, sizeof(*to));
}

/**
* Handles message receiving over UDP
* @param message data buffer
* @param max_size max size of bytes that can be written in buffer
* @param to destination node
* @param from client information
* @return recvfrom function result
*/
int32_t recv_udp_message(void* const message, size_t max_size, SOCKET_T to, SOCKADDR_IN_T* from)
{
	SOCKLEN_T len = sizeof(*from);
	return recvfrom(to, message, max_size, 0, (SOCKADDR_T *) from, &len);
}

/**
* Closes socket
* @param socket
*/
void finalize_udp(SOCKET_T socket)
{
	CLOSESOCKET(socket);
#if defined(WIN32)
	WSACleanup();
#endif
}

#pragma endregion /* UDP UTILS */

#include "basic_networking.h"

#include <stdint.h>
#include <stdio.h>

#pragma region UDP_UTILS

uint8_t initializeClientConnection(SOCKADDR_IN_T* server, SOCKET_T* client, const char* hostname, uint32_t port)
{
	SOCKET_HEAD_INIT

	*client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!CHECK_VALID(*client))
		return ERR_INVALID;

	ADDRINFO hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_DGRAM;
	hints.ai_protocol	= IPPROTO_UDP;

	ADDRINFO* res;
	char port_s[16] = { 0 };

	_itoa_s((uint32_t) port, port_s, sizeof(port_s), 10);

	if (getaddrinfo(hostname, port_s, &hints, &res) != 0)
	{
		return ERR_GETADDRINFO_FAIL;
	}

	memcpy(server, (struct SOCKADDR_IN_T*) res->ai_addr, res->ai_addrlen);

	freeaddrinfo(res);
	return SUCCESS;
}


uint8_t initializeServerSocket(SOCKADDR_IN_T* server, SOCKET_T* server_socket, uint32_t port)
{
	SOCKET_HEAD_INIT

	if (!CHECK_VALID(*server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
		return ERR_INVALID;
	}

	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	server->sin_addr.s_addr = INADDR_ANY;

	int code = bind(*server_socket, (struct SOCKADDR_T*)server, sizeof(*server));

	if (!CHECK_VALID(code)) {
		return ERR_COULD_NOT_BIND;
	}

	return SUCCESS;
}


int32_t send_message(const char* message, size_t size, SOCKET_T from, SOCKADDR_IN_T* to)
{
	return sendto(from, message, size, 0, (struct SOCKADDR_T*) to, sizeof(*to));
}

int32_t recv_message(char** message, size_t max_size, SOCKET_T to, SOCKADDR_IN* from)
{
	size_t len = sizeof(*from);
	return recvfrom(to, message, max_size, 0, (struct SOCKADDR_T*) from, &len);
}

void finalize(SOCKET_T* socket)
{
#if defined(WIN32)
	CLOSESOCKET(socket);
	WSACleanup();
#endif
}

#pragma endregion

#pragma region TCP_UTILS



#pragma endregion
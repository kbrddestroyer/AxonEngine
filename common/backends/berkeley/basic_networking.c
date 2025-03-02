#include "basic_networking.h"

#include <stdio.h>
#include <string.h>

#pragma region UDP_UTILS

uint8_t connect_udp_client(SOCKADDR_IN_T* server, SOCKET_T* client, const char* hostname, uint32_t port)
{
	SOCKET_HEAD_INIT

	if (!CHECK_VALID(*client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
		return ERR_INVALID;

	ADDRINFO_T hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_DGRAM;
	hints.ai_protocol	= IPPROTO_UDP;

	ADDRINFO_T* res;
	
	char port_s[16] = { 0 };
	snprintf(port_s, sizeof(port_s), "%d", port);

	if (getaddrinfo(hostname, port_s, &hints, &res) != 0)
	{
		return ERR_GETADDRINFO_FAIL;
	}

	memcpy(server, (SOCKADDR_IN_T*) res->ai_addr, res->ai_addrlen);

#if defined(WIN32)
	DWORD read_timeout = 10;  // it is milliseconds!
	setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, (char const*)&read_timeout, sizeof read_timeout);
#else

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;

	setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

#endif
	freeaddrinfo(res);
	return SUCCESS;
}


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

#if defined(WIN32)
	DWORD read_timeout = 10;  // it is milliseconds!
	setsockopt(*server_socket, SOL_SOCKET, SO_RCVTIMEO, (char const*)&read_timeout, sizeof read_timeout);
#else

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;

	setsockopt(*server_socket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

#endif
	return SUCCESS;
}

int32_t send_udp_message(const char* message, size_t size, SOCKET_T from, const SOCKADDR_IN_T* to)
{
	return sendto(from, message, size, 0, (SOCKADDR_T*) to, sizeof(*to));
}

int32_t recv_udp_message(char** message, size_t max_size, SOCKET_T to, SOCKADDR_IN_T* from)
{
	size_t len = sizeof(*from);
	return recvfrom(to, message, max_size, 0, (SOCKADDR_T*) from, &len);
}

void finalize_udp(SOCKET_T socket)
{
	CLOSESOCKET(socket);
#if defined(WIN32)
	WSACleanup();
#endif
}

#pragma endregion /* UDP UTILS */



#pragma region TCP_UTILS



#pragma endregion /* TCP UTILS */

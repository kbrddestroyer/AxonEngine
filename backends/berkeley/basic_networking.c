#include "basic_networking.h"

#include <stdint.h>

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

	memcpy(server, (PSOCKADDR_IN)res->ai_addr, res->ai_addrlen);

	freeaddrinfo(res);
	return SUCCESS;
}


uint8_t initializeServerSocket(SOCKADDR_IN_T* server, SOCKET_T* server_socket, uint32_t port)
{
	SOCKET_HEAD_INIT

	if (!CHECK_VALID(server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
		return ERR_INVALID;
	}

	memset((char*)server, 0, sizeof(*server));
	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	server->sin_addr.s_addr = INADDR_ANY;

	if (!CHECK_VALID(bind(server_socket, (SOCKADDR_T *)server, sizeof(*server)))) {
		return ERR_COULD_NOT_BIND;
	}

	return SUCCESS;
}


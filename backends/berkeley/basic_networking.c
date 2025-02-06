#include "basic_networking.h"

#include <stdint.h>

uint8_t initialize(SOCKADDR_IN_T* server, const char* hostname, uint32_t port)
{
#if defined(_WIN32)
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		return ERR_WININIT_FAIL;
	}
#endif
	SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!CHECK_VALID(client_socket))
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

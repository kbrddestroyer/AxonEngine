#include "tcp_connection.h"
#include <string.h>

uint8_t connect_tcp_client(SOCKADDR_IN_T* server, SOCKET_T* client, const char* hostname, uint32_t port)
{
	SOCKET_HEAD_INIT

	if (!CHECK_VALID(*client = socket(AF_INET, SOCK_STREAM, 0)))
		return ERR_INVALID;

	ADDRINFO_T hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	ADDRINFO_T* res;

	char port_s[16] = { 0 };
	snprintf(port_s, sizeof(port_s), "%d", port);

	if (getaddrinfo(hostname, port_s, &hints, &res) != 0)
	{
		return ERR_GETADDRINFO_FAIL;
	}

	memcpy(server, (SOCKADDR_IN_T*)res->ai_addr, res->ai_addrlen);

	SET_ASYNC_SOCKET(10, *client);

	if (!CHECK_VALID(connect(*client, (SOCKADDR_T*) server, sizeof(*server))))
	{
		return ERR_CONNECTION_ABORTED;
	}

	freeaddrinfo(res);
	return SUCCESS;
}


uint8_t create_tcp_server(SOCKADDR_IN_T* server, SOCKET_T* server_socket, uint32_t port)
{
	SOCKET_HEAD_INIT

		if (!CHECK_VALID(*server_socket = socket(AF_INET, SOCK_STREAM, 0))) {
			return ERR_INVALID;
		}

	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	server->sin_addr.s_addr = INADDR_ANY;

	if (!CHECK_VALID(bind(*server_socket, (SOCKADDR_T*)server, sizeof(*server)))) {
		return ERR_COULD_NOT_BIND;
	}
	
	if (!CHECK_VALID(listen(*server_socket, 1024)))
	{
		return ERR_COULD_NOT_BIND;
	}

	return SUCCESS;
}

int32_t send_tcp_message(const char* message, size_t size, SOCKET_T from)
{
	return send(from, message, size, 0);
}

int32_t recv_tcp_message(char* const message, size_t max_size, SOCKET_T c_sock)
{
	return recv(c_sock, message, max_size, 0);
}

SOCKET_T accept_incoming(SOCKET_T server, SOCKADDR_IN_T* c_addr)
{
	SOCKLEN_T addr_s = (SOCKLEN_T) sizeof(*c_addr);
	return accept(server, (SOCKADDR_T*) c_addr, &addr_s);
}

void finalize_tcp(SOCKET_T socket)
{
	CLOSESOCKET(socket);
#if defined(WIN32)
	WSACleanup();
#endif
}
#include "tcp_connection.h"
#include <string.h>

/**
 * Accept client connection to server
 * @param server server socket
 * @param [out] c_addr client information
 * @return
 */
SOCKET_T accept_incoming(const SOCKET_T server, const SOCKADDR_IN_T *c_addr)
{
	SOCKLEN_T addr_s = sizeof(*c_addr);
	return accept(server, (SOCKADDR_T*) c_addr, &addr_s);
}

/**
* Connects to the remote host via TCP. Uses `getaddrinfo`.
* @param [out] server server SOCKADDR_IN_T structure
* @param client bound client socket
* @param hostname ip/hostname of a target node
* @param port port of a target node
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
uint8_t connect_tcp_client(SOCKADDR_IN_T* server, SOCKET_T* client, const char* hostname, uint32_t port)
{
	SOCKET_HEAD_INIT

	if (!CHECK_VALID(*client = socket(AF_INET, SOCK_STREAM, 0)))
		return ERR_INVALID;

	ADDRINFO_T hints = {0};

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

/**
* Initializes server socket with TCP protocol.
* @param [out] server server information (SOCKADDR_IN_T structure)
* @param server_socket bound server socket
* @param port port to create server onto
* @returns 0
* @returns ERR_CODE (defined in basic_networking.h)
*/
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

/**
* Sends data over TCP
* @param message data to send, sequence of bytes
* @param size size of message, bytes. Most of the time can be strlen(message), but we cannot be sure in case, when message is serialized data
* @param from source socket
* @returns send() function result
*/
int32_t send_tcp_message(const void* message, const size_t size, const SOCKET_T from)
{
	return send(from, message, size, 0);
}

/**
* Handles message receiving over TCP
* @param message data buffer, must be either a static array or pre-allocated
* @param max_size max size of bytes that can be written in buffer
* @param c_sock client socket to receive message from
* @returns c_sock result
*/
int32_t recv_tcp_message(void* const message, const size_t max_size, const SOCKET_T c_sock)
{
	return recv(c_sock, message, max_size, 0);
}

/**
* Closes socket, on Windows machines also cleanups WSA
* @param socket socket to close
*/
void finalize_tcp(const SOCKET_T socket)
{
	CLOSESOCKET(socket);
#if defined(WIN32)
	WSACleanup();
#endif
}
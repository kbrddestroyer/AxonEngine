#include "winclient.h"

#if defined(_WIN32)
#include <ws2tcpip.h>
#include <AxonError.h>

Axon::Backends::Windows::WinUDPClient::WinUDPClient(char* hostname, Axon::Connection::AXON_PORT port) :
	Axon::Connection::ClientConnectionHandler(hostname, port) {
	memset(&client_socket, 0, sizeof(client_socket));
	memset(&server, 0, sizeof(server));
	memset(&ws, 0, sizeof(ws));
}

Axon::Backends::Windows::WinUDPClient::~WinUDPClient()
{
	closesocket(client_socket);
	WSACleanup();
}

bool Axon::Backends::Windows::WinUDPClient::Initialize()
{
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		throw Axon::AxonError(Axon::Error::AxonErrorCode::INTERNAL_ERROR);
	}

	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		return false;
	}

	addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	addrinfo* res;
	char port_s[16] = { 0 };
	_itoa_s((uint32_t) port, port_s, 10);
	
	if (getaddrinfo(hostname, port_s, &hints, &res) != 0)
	{
		return false;
	}

	memcpy(&server, (PSOCKADDR_IN) res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	return true;
}

void Axon::Backends::Windows::WinUDPClient::SendUDPMessage(char* serialized, size_t size)
{
	sendto(client_socket, serialized, size, 0, (sockaddr*)&server, sizeof(SOCKADDR_IN)) != SOCKET_ERROR;
}
#endif

#include "winclient.h"
#include <AxonEngine.h>

#if defined(_WIN32)

Axon::Backends::Windows::WinUDPClient::WinUDPClient(char* hostname, Axon::Connection::AXON_PORT port) :
	Axon::Client::ClientConnectionHandler(hostname, port) {}

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

	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(this->port);
	server.sin_addr.S_un.S_addr = inet_addr(hostname);

	return true;
}

void Axon::Backends::Windows::WinUDPClient::SendUDPMessage(char* serialized, size_t size)
{
	sendto(client_socket, serialized, size, 0, (sockaddr*)&server, sizeof(SOCKADDR_IN)) != SOCKET_ERROR;
}
#endif

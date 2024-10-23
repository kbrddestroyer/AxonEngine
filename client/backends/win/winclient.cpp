#include "winclient.h"

bool Axon::Backends::Windows::WinUDPClient::Startup()
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
	server.sin_port = htons(port);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	const char* buffer = "Hello";

	if (sendto(client_socket, buffer, strlen(buffer), 0, (sockaddr*)&server, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		throw Axon::AxonError(Axon::Error::AxonErrorCode::INTERNAL_ERROR);
	}
	closesocket(client_socket);
	WSACleanup();
	return true;
}

void Axon::Backends::Windows::WinUDPClient::SendTo()
{
}

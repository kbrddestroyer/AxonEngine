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
	int32_t len = sizeof(server);

	Axon::Connection::UDPMessage message;
	message.data = new char[sizeof(uint32_t)];
	
	uint32_t* data = (uint32_t*)message.data;
	*data = 1;

	message.size = sizeof(uint32_t);
	message.tag = 1000;

	SendUserMessage(message);
	
	delete[] message.data;
	std::shared_ptr<char[]> buffer(new char[1024]);
	size_t size;

	if ((size = recvfrom(client_socket, buffer.get(), 1024, 0, (SOCKADDR*)&server, &len)) != SOCKET_ERROR)
	{
		printf("Got message");
		return true;
	}
	return true;
}

bool Axon::Backends::Windows::WinUDPClient::SendUserMessage(Axon::Connection::UDPMessage message)
{
	size_t size;
	std::shared_ptr<char[]> serialized = message.getSerializedData(size);
	return sendto(client_socket, serialized.get(), size, 0, (sockaddr*)&server, sizeof(SOCKADDR_IN)) != SOCKET_ERROR;
}

#endif

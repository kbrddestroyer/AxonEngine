#include <Axon.h>

#include "wintransport.h"
#include "wintransport.h"
#include <iostream>
#include <AxonEngine.h>

#include <iostream>

#if defined(WINDOWS_PLATFORM)
Axon::Backends::Windows::WinUDPConnectionHandler::WinUDPConnectionHandler(Axon::Connection::AXON_PORT port)
{
    this->port = port;
}

Axon::Backends::Windows::WinUDPConnectionHandler::~WinUDPConnectionHandler()
{
    closesocket(server_socket);
    WSACleanup();
}

bool Axon::Backends::Windows::WinUDPConnectionHandler::Initialize()
{
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        throw Axon::AxonError(Axon::Error::AxonErrorCode::INTERNAL_ERROR);
    }
    
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        return false;
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool Axon::Backends::Windows::WinUDPConnectionHandler::SendUserMessage(char* serialized, size_t size, uint64_t connectionID)
{
    SOCKADDR_IN client = connections[connectionID];

    return sendto(server_socket, serialized, size, 0, (SOCKADDR*) &client, sizeof(client)) != SOCKET_ERROR;
}

void Axon::Backends::Windows::WinUDPConnectionHandler::Listen()
{
    char buffer[SERVER_PACKAGE_MAXSIZE];

    SOCKADDR_IN client;
    int32_t len = sizeof(client);
    int32_t size;
    while (isRunning) {
        if ((size = recvfrom(server_socket, buffer, SERVER_PACKAGE_MAXSIZE, 0, (SOCKADDR*)&client, &len)) != SOCKET_ERROR)
        {
            buffer[size] = 0;
            std::cout << "Recfrom: " << buffer << " | " << inet_ntoa(client.sin_addr) << std::endl;
        }
        else
        {
            std::cout << WSAGetLastError() << std::endl;
            isRunning = false;
        }
    }
}
#endif
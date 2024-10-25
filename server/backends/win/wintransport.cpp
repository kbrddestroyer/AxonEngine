#include "wintransport.h"
#include <iostream>
#include <AxonEngine.h>

#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__NT__)
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

bool Axon::Backends::Windows::WinUDPConnectionHandler::SendUserMessage(Axon::Connection::ServerUDPMessage)
{
    return false;
}

void Axon::Backends::Windows::WinUDPConnectionHandler::Listen()
{
    char buffer[1024];
    SOCKADDR_IN client;
    int32_t len = sizeof(client);
    int32_t size;
    while (isRunning) {
        memset(buffer, 0, sizeof(buffer));
        if ((size = recvfrom(server_socket, buffer, 1024, 0, (SOCKADDR*)&client, &len)) != SOCKET_ERROR)
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
#include "wintransport.h"
#include <iostream>
#include "AxonEngine.h"


#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__NT__)

Axon::Windows::WinUDPSocket::WinUDPSocket(const char* addr, uint16_t port)
{
    this->addr = addr;
    this->port = port;
    isRunning = true;

    if (!Startup())
    {
        std::cout << "Could not start" << std::endl;
    }
}

Axon::Windows::WinUDPSocket::~WinUDPSocket()
{
    closesocket(server_socket);
    WSACleanup();
}

bool Axon::Windows::WinUDPSocket::Startup()
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

void Axon::Windows::WinUDPSocket::Listen()
{
    while (isRunning) {
        char* buffer;

    }
}

#endif
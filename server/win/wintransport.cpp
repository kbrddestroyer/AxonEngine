#include "wintransport.h"
#include <iostream>
#include <AxonEngine.h>


#ifdef _WIN32 || _WIN64

Axon::Windows::WinUDPSocket::WinUDPSocket(const char* addr, uint16_t port)
{
    this->addr = addr;
    this->port = port;

    if (!Startup())
    {
        std::cout << "Could not start" << std::endl;
    }
}

Axon::Windows::WinUDPSocket::~WinUDPSocket()
{
    closesocket(client_socket);
    WSACleanup();
}

bool Axon::Windows::WinUDPSocket::Startup()
{
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        throw Axon::AxonError(Axon::Error::AxonErrorCode::INTERNAL_ERROR);
    }
    
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        return false;
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.S_un.S_addr = inet_addr(addr);

    return true;
}

#endif
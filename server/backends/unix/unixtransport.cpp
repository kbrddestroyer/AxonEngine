#include "unixtransport.h"

#if defined(__unix__) || __APPLE__
#include <iostream>

Axon::Backends::Unix::UnixUDPConnectionHandler::UnixUDPConnectionHandler(Axon::Connection::AXON_PORT port) {
    this->port = port;
}

bool Axon::Backends::Unix::UnixUDPConnectionHandler::Initialize()
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        throw AxonError(Error::AxonErrorCode::INTERNAL_ERROR);
    }

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*) &server, sizeof(server)) < 0)
    {
        return false;
    }

    return true;
}

void Axon::Backends::Unix::UnixUDPConnectionHandler::Listen()
{
    char buffer[1024];
    socklen_t len = sizeof(client);

    while (isRunning)
    {
        ssize_t size;
        if ((size = recvfrom(sockfd, &buffer, 1024, MSG_WAITALL, (sockaddr*) &client, &len)) < 0)
        {
            std::cout << "Critical" << std::endl;
            isRunning = false;
        }

        buffer[size] = 0;
        std::cout << buffer << std::endl;
    }
}

void Axon::Backends::Unix::UnixUDPConnectionHandler::SendMessage(Axon::Connection::ServerUDPMessage message) {

}
#endif

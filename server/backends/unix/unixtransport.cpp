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
    char* buffer = new char[1024];

    std::shared_ptr<char[]> serialized(buffer);

    socklen_t len = sizeof(client);
    while (isRunning)
    {
        ssize_t size;
        if ((size = recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (sockaddr*) &client, &len)) < 0)
        {
            std::cout << "Critical" << std::endl;
            isRunning = false;
        }
    }
}

bool Axon::Backends::Unix::UnixUDPConnectionHandler::SendUserMessage(char* serialized, size_t size, uint64_t connectionID) {
    return sendto(sockfd, serialized, size, MSG_CTRUNC, (struct sockaddr*) &client, sizeof(client)) > 0;
}
#endif

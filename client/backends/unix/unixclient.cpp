//
// Created by Aleksey Grudko on 24.10.24.
//

#include "unixclient.h"
#include <string>
#include <iostream>


Axon::Backends::Unix::UnixUDPClient::UnixUDPClient(char *hostname, Axon::Connection::AXON_PORT port) {
    this->port = port;
    this->hostname = hostname;
}

bool Axon::Backends::Unix::UnixUDPClient::Initialize() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        throw AxonError(Error::AxonErrorCode::INTERNAL_ERROR);
    }

    memset(&server, 0, sizeof(server));

    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(hostname);

    char buffer[] = "Hello World!";
    std::cout << "sending" << std::endl;
    if (sendto(sockfd, buffer, strlen(buffer), MSG_CTRUNC, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        return false;
    }

    close(sockfd);
    return true;
}
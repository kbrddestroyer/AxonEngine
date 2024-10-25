#include "unixclient.h"

#if defined(__unix__) || __APPLE__
#include <string>
#include <iostream>


Axon::Backends::Unix::UnixUDPClient::UnixUDPClient(char *hostname, Axon::Connection::AXON_PORT port) :
    Axon::Client::ClientConnectionHandler(hostname, port) {
    sockfd = 0;
    server = {};
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

    Axon::Connection::UDPMessage message;
    message.data = "Hello World!";
    message.size = strlen(message.data);
    message.tag = 1;

    SendUserMessage(message);

    char* buffer = new char[1024];
    size_t size;
    socklen_t len = sizeof(server);

    if ((size = recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (sockaddr*) &server, &len)) < 0)
    {
        std::cout << "Critical" << std::endl;
        return false;
    }

    buffer[size] = 0;

    std::shared_ptr<char[]> serialized (buffer);

    message.setDeserialized(serialized, size);
    std::cout << message.data << " " << message.tag << std::endl;

    return true;
}

bool Axon::Backends::Unix::UnixUDPClient::SendUserMessage(Axon::Connection::UDPMessage message)
{
    size_t serialized_size;
    std::shared_ptr<char[]> serialized = message.getSerializedData(serialized_size);

    return sendto(sockfd, serialized.get(), serialized_size, MSG_CTRUNC, (struct sockaddr*) &server, sizeof(server)) < 0;
}

Axon::Backends::Unix::UnixUDPClient::~UnixUDPClient() {
    close(sockfd);
}
#endif

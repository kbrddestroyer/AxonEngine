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

        buffer[size] = 0;
        Axon::Connection::UDPMessage deserialized;
        deserialized.setDeserialized(serialized, size);

        Axon::Connection::ServerUDPMessage message;
        message.inet_addr = inet_ntoa(client.sin_addr);

        std::cout << message.inet_addr << " " << deserialized.data << std::endl;

        message.payload.data = "SERVER_RESPONDED";
        message.payload.size = strlen(message.payload.data);
        message.payload.tag = deserialized.tag;

        SendMessage(message);
    }
}

bool Axon::Backends::Unix::UnixUDPConnectionHandler::SendMessage(Axon::Connection::ServerUDPMessage message) {
    size_t serialized_size;
    std::shared_ptr<char[]> serialized = message.payload.getSerializedData(serialized_size);

    return sendto(sockfd, serialized.get(), serialized_size, MSG_CTRUNC, (struct sockaddr*) &client, sizeof(client)) > 0;
}
#endif

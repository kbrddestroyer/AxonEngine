#include "client_connection.h"


Axon::Client::ClientConnectionHandler::ClientConnectionHandler(char* hostname, Axon::Connection::AXON_PORT port)
{
    this->port = port;
    this->hostname = hostname;
}

bool Axon::Client::ClientConnectionHandler::SendUserMessage(const Axon::Connection::UDPMessage& message)
{
    size_t serializedSize;
    char* serialized = serialize(message.data, message.size, message.tag, &serializedSize);

    this->SendUDPMessage(serialized, serializedSize);
    free(serialized);

    return true;
}

bool Axon::Client::ClientConnectionHandler::Startup() {
    return Initialize();
}

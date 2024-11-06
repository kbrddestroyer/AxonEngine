#include "client_connection.h"

#ifndef AXON_CLIENT
#define AXON_CLIENT
#endif

#include <Axon.h>
#include <AxonBackend.h>


Axon::Connection::ClientConnectionHandler::ClientConnectionHandler(char* hostname, Axon::Connection::AXON_PORT port)
{
    this->port = port;
    this->hostname = hostname;
}

bool Axon::Connection::ClientConnectionHandler::SendUserMessage(const Axon::Connection::UDPMessage& message)
{
    size_t serializedSize;
    char* serialized = serialize(message.data, message.size, message.tag, &serializedSize);

    this->SendUDPMessage(serialized, serializedSize);
    free(serialized);

    return true;
}

bool Axon::Connection::ClientConnectionHandler::Startup() {
    return Initialize();
}

Axon::Connection::ClientConnectionHandler* Axon::Connection::ClientConnectionHandler::createClientHandler(char* hostname, Axon::Connection::AXON_PORT port)
{
#if defined(WINDOWS_PLATFORM)
    return new Axon::Backends::Windows::WinUDPClient(hostname, port);
#elif defined(UNIX_PLATFORM)
    return new Axon::Backends::Unix::UnixUDPClient(hostname, port);
#else
    return nullptr;
#endif
}

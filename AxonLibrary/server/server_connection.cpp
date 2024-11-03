#include "server_connection.h"

#ifndef AXON_SERVER
#define AXON_SERVER
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <common/serialization/serialization.h>
#ifdef __cplusplus
}
#endif

#include <AxonBackend.h>

Axon::Connection::ServerConnectionHandler::ServerConnectionHandler(uint16_t port) : port(port)
{
    isRunning = false;
}

Axon::Connection::ServerConnectionHandler::~ServerConnectionHandler() = default;

bool Axon::Connection::ServerConnectionHandler::Running() const
{
    return isRunning;
}

void Axon::Connection::ServerConnectionHandler::Startup()
{
    isRunning = Initialize();
    Listen();
}


constexpr void Axon::Connection::ServerConnectionHandler::OnIncomingMessage(const ServerUDPMessage& message)
{

}

constexpr void Axon::Connection::ServerConnectionHandler::OnIncomingConnection(const ServerUDPMessage& message)
{

}

bool Axon::Connection::ServerConnectionHandler::SendUDPMessage(const ServerUDPMessage& message)
{
    size_t size;
    char* serialized = serialize(message.payload.data, message.payload.size, message.payload.tag, &size);
    this->SendUserMessage(serialized, size, message.connectionID);
    free(serialized);

    return true;
}

Axon::Connection::ServerConnectionHandler* Axon::Connection::ServerConnectionHandler::createServerHandler(Axon::Connection::AXON_PORT port)
{
#if defined(WINDOWS_PLATFORM)
    return new Axon::Backends::Windows::WinUDPConnectionHandler(port);
#elif defined(UNIX_PLATFORM)
    return new Axon::Backends::Unix::UnixUDPConnectionHandler(port);
#else
#endif
}
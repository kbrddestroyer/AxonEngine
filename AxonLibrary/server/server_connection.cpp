#include "server_connection.h"
#include "server_connection.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <common/serialization/serialization.h>
#ifdef __cplusplus
}
#endif

Axon::Connection::ServerConnectionHandler::ServerConnectionHandler(uint16_t port) : port(port)
{
    isRunning = false;
}

Axon::Connection::ServerConnectionHandler::~ServerConnectionHandler() = default;

bool Axon::Connection::ServerConnectionHandler::Running() const
{
    return isRunning;
}

void Axon::Connection::ServerConnectionHandler::Start()
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

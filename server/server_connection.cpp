#include "server_connection.h"
#include "server_connection.h"
#include "server_connection.h"

Axon::Connection::ServerConnectionHandler::ServerConnectionHandler(uint16_t port) : port(port)
{
    isRunning = false;
}

bool Axon::Connection::ServerConnectionHandler::Running() const
{
    return isRunning;
}

void Axon::Connection::ServerConnectionHandler::Start()
{
    Listen();
}

void Axon::Connection::ServerConnectionHandler::OnIncomingConnection(ServerUDPMessage message)
{
    mConnections[mConnections.size()] = {message.inet_addr};
}

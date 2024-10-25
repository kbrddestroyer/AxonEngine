#include "server_connection.h"

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


void Axon::Connection::ServerConnectionHandler::OnIncomingMessage(ServerUDPMessage message)
{

}

void Axon::Connection::ServerConnectionHandler::OnIncomingConnection(ServerUDPMessage message)
{

}

#include "server_connection.h"


Axon::ServerConnectionHandler::ServerConnectionHandler(uint16_t port) : port(port)
{
    isRunning = false;
}

bool Axon::ServerConnectionHandler::Running() const
{
    return isRunning;
}
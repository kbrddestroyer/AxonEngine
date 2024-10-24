#include "client_connection.h"


Axon::Client::ClientConnectionHandler::ClientConnectionHandler(char* hostname, Axon::Connection::AXON_PORT port)
{
    this->port = port;
    this->hostname = hostname;
}

bool Axon::Client::ClientConnectionHandler::Startup() {
    return Initialize();
}

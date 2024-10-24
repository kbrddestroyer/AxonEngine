#include "client_connection.h"


Axon::Client::ClientConnectionHandler::ClientConnectionHandler(char* hostname, Axon::Connection::AXON_PORT port) :
    hostname(hostname), port(port)
{}

bool Axon::Client::ClientConnectionHandler::Startup() {
    return Initialize();
}

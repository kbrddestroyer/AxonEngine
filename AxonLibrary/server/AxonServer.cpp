#include "AxonServer.h"
#include <iostream>


Axon::Server::Server()
{
	handler = Axon::Connection::ServerConnectionHandler::createServerHandler(10243);
	std::cout << "Started server" << std::endl;
	handler->Startup();
}

Axon::Server::~Server()
{
	delete handler;
}

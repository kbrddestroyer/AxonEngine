#include "AxonClient.h"


Axon::Client::Client(char* hostname, Axon::Connection::AXON_PORT port)
{
	handler = Axon::Connection::ClientConnectionHandler::createClientHandler(hostname, port);
	handler->Startup();
}

Axon::Client::~Client()
{
	delete handler;
}

void Axon::Client::SendUserMessage(Axon::Connection::Message& message)
{
	handler->SendUserMessage(message);
}

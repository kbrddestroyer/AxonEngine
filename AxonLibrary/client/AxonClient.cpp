#include "AxonClient.h"
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

void Axon::Client::SendUserMessage(Axon::Connection::UDPMessage& message)
{
	handler->SendUserMessage(message);
}

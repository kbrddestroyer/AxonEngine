#include "AxonClient.h"

Axon::Client::Client(char* hostname, Axon::Connection::AXON_PORT port)
{
	handler = Axon::Connection::ClientConnectionHandler::createClientHandler(hostname, port);

	handler->Startup();

	const char* message_data = "Hello from Axon client";

	Axon::Connection::UDPMessage message;
	Axon::Connection::UDPMessage::createUDPMessage(message, (void*) message_data, strlen(message_data), 1);
	handler->SendUserMessage(message);
}

Axon::Client::~Client()
{
	delete handler;
}

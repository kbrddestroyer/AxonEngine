#include "AxonServer.h"
#include <iostream>


Axon::Server::Server()
{
	std::cout << "Started server" << std::endl;
}

void Axon::Server::Start()
{
	handler = Axon::Connection::ServerConnectionHandler::createServerHandler(10243);
	handler->Startup(Axon::Server::onMessageRecivedCall, this);
}

Axon::Server::~Server()
{
	delete handler;
}

void Axon::Server::Subscribe(SERVER_CALLBACK_FUNC callback)
{
	calls.push_back(callback);
}

void Axon::Server::onMessageRecived(Axon::Connection::ServerUDPMessage message)
{
	for (SERVER_CALLBACK_FUNC callback : calls)
	{
		callback(message);
	}
}

void Axon::Server::onMessageRecivedCall(Axon::Connection::ServerUDPMessage message, void* owner)
{
	Server* server = reinterpret_cast<Server*>(owner);
	server->onMessageRecived(message);
}

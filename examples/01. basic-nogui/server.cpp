#ifndef AXON_SERVER
#define AXON_SERVER
#endif

#include <AxonEngine.h>
#include <iostream>

void onMessageRecived(Axon::Connection::ServerUDPMessage nessage)
{
	std::cout << "RECV CALLBACK FUCKING WORKS!" << std::endl;
}


int main()
{
	Axon::Server server = Axon::Server();

	server.Subscribe(onMessageRecived);
	server.Start();
}

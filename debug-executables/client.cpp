/*
* Just a test executable
* I'll use it in dockertest process
* 
* Falls in infinite loop and sends UDP packages every second
* Can be stopped by keyboard interrupt (Ctrl+C)
* 
* By: KbrdDestroyer
*/

#define AXON_CLIENT

#include <Axon.h>
#include <AxonClient.h>

#include <iostream>

using namespace Axon;

int main()
{
	const char* hostname = "localhost";
	const uint16_t port = 10243;

    Client::ClientConnectionHandler* client;
#if defined(_WIN32)
	client = new Backends::Windows::WinUDPClient((char*) hostname, port);
#elif __APPLE__ || defined(__unix__)
    client = new Backends::Unix::UnixUDPClient((char*) hostname, port);
#endif
	std::cout << "[WARNING] This file should be used by docker-test" << std::endl;
	
	if (!client->Startup())
	{
		std::cerr << "Could not start client. Check your backend code" << std::endl;
		return 1;
	}
	
	const char* message_data = "Hello World";

	for (uint16_t tag = 0; tag < 10; tag++)
	{
		Axon::Connection::UDPMessage message;
		Axon::Connection::UDPMessage::createUDPMessage(message, (void*)message_data, strlen(message_data), tag);

		client->SendUserMessage(message);
		std::cout << "[axon-client -> axon-server] Message dispatched " << std::endl;

#if defined(_WIN32)
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	return 0;
}

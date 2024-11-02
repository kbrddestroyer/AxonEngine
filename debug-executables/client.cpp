#define AXON_CLIENT

#include <Axon.h>
#include <AxonClient.h>

#include <iostream>

using namespace Axon;

int main()
{
    Client::ClientConnectionHandler* client;
#if defined(_WIN32)
	client = new Backends::Windows::WinUDPClient((char*) "127.0.0.1", 10243);
#elif __APPLE__ || defined(__unix__)
    client = new Backends::Unix::UnixUDPClient("127.0.0.1", 10243);
#endif

	const char* message_test = "Hello, I'm serialized";

	if (!client->Startup())
	{
		std::cout << "Could not start" << std::endl;
		return 1;
	}
	
	const char* message_data = "Hello World";

	Axon::Connection::UDPMessage message;
	Axon::Connection::UDPMessage::createUDPMessage(message, (void*) message_data, sizeof(message_data), 1);

	client->SendUserMessage(message);

	std::cout << "Test passed: message dispatched" << std::endl;
	std::cin.get();
	return 0;
}

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
#elif __APPLE__
    client = new Backends::Unix::UnixUDPClient("127.0.0.1", 10243);
#endif

	const char* message_test = "Hello, I'm serialized";

	if (!client->Startup())
	{
		std::cout << "Could not start" << std::endl;
	}
	else
	{
		std::cout << "Success!" << std::endl;
	}
	std::cin.get();
	return 0;
}

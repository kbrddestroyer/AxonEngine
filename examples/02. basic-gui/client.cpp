#ifndef AXON_CLIENT
#define AXON_CLIENT
#endif

#include <AxonEngine.h>


int main()
{
	Axon::Client client = Axon::Client((char*)"localhost", 10243);


	const char* message_data = "Hello from Axon client";

	Axon::Connection::Message message = Axon::Connection::Message::createUDPMessage((void*)message_data, strlen(message_data), 1);

	for (uint16_t index = 0; index < 8; index++)
	{
		client.SendUserMessage(message);
	}
	return 0;
}

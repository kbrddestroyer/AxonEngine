/*
* Just a test executable
* I'll use it in dockertest process
* 
* Falls in infinite loop and sends UDP packages every second
* Can be stopped by keyboard interrupt (Ctrl+C)
* 
* By: KbrdDestroyer
*/

#include <AxonEngine.h>

using namespace Axon;

int main()
{
	Axon::Client client = Axon::Client((char*) "localhost", 10243);


	const char* message_data = "Hello from Axon client";

	Axon::Connection::UDPMessage message = Axon::Connection::UDPMessage::createUDPMessage((void*)message_data, strlen(message_data), 1);
	
	for (uint16_t index = 0; index < 8; index++)
	{
		client.SendUserMessage(message);
	}
	return 0;
}

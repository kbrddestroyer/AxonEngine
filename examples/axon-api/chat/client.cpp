#include <iostream>
#include <AxonNetwork.hpp>
#include <string.h>


void initConnection(Networking::AsyncSynapse& synapse)
{
	char* QQ = "QQ";
	Networking::AxonMessage message(reinterpret_cast<void*>(QQ), strlen(QQ) + 1, 1);

	synapse.send(message);
}

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
{
	Networking::AxonMessage message(event.getMessage());

	std::cout << reinterpret_cast<char*>(message.getMessage()) << std::endl;
}

int main()
{
	Networking::AsyncSynapse synaps({ "localhost", 10423 });

	initConnection(synaps);
	synaps.start();

	synaps.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);

	char buffer[1024];
	while (synaps.alive())
	{
		std::cin.getline(buffer, 1024, '\n');

		Networking::AxonMessage message(
			buffer,
			strlen(buffer) + 1,
			0
		);
		synaps.send(message);
	}
}
#include <iostream>
#include <AxonNetwork.hpp>
#include <string>


void initConnection(Networking::AsyncSynaps& synaps)
{
	char* QQ = "QQ";
	Networking::AxonMessage message(reinterpret_cast<void*>(QQ), strlen(QQ) + 1, 1);

	synaps.send(message);
}

void onMessageReceived(const Networking::SynapsMessageReceivedEvent& event)
{
	Networking::AxonMessage message(event.getMessage());

	std::cout << reinterpret_cast<char*>(message.getMessage()) << std::endl;
}

int main()
{
	Networking::AsyncSynaps synaps({ "localhost", 10423 });

	initConnection(synaps);
	synaps.start();

	synaps.getEventManager().subscribe<Networking::SynapsMessageReceivedEvent>(onMessageReceived);

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
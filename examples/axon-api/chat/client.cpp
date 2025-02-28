#include <iostream>
#include <AxonNetwork.hpp>
#include <string>


void initConnection(Networking::AsyncSynaps& synaps)
{
	const char* QQ = "QQ";
	Networking::Message message(QQ, strlen(QQ) + 1, 1);

	synaps.send(message.toMessage());
}

void onMessageReceived(Networking::SynapsMessageReceivedEvent* event)
{
	Networking::Message message(event->getMessage());

	std::cout << reinterpret_cast<char*>(message.getBitstream()) << std::endl;
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

		Networking::Message message(
			buffer,
			strlen(buffer) + 1,
			0
		);
		synaps.send(message.toMessage());
	}
}
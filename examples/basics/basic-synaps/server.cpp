#include <networking/AxonNetwork.hpp>

#include <iostream>

void onMessageReceived(Networking::SynapsMessageReceivedEvent* event)
{
	std::cout << event->getMessage().getMessage() << std::endl;
}

int main()
{
	Networking::Synaps serverConnection = Networking::Synaps(10423);
	serverConnection.getEventManager().subscribe<Networking::SynapsMessageReceivedEvent>(onMessageReceived);

	std::cout << "Starting Synaps connection" << std::endl;
	serverConnection.listen();
}
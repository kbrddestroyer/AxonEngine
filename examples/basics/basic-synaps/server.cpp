/*
* This example shows basic connection, using Axon internal tools
* Synaps provides connection between two points
* 
* This file defines basic server logic, with message handling using Synaps event system
*/

#include <networking/AxonNetwork.hpp>
#include <iostream>

void onMessageReceived(const Networking::SynapsMessageReceivedEvent& event)
{
	std::cout << event.getMessage().getMessage() << std::endl;
}

int main()
{
	Networking::AsyncSynaps serverConnection = Networking::AsyncSynaps(10423);
	Networking::AsyncSynaps internalMessageTool = Networking::AsyncSynaps({ "localhost", 10423 });
	serverConnection.getEventManager().subscribe<Networking::SynapsMessageReceivedEvent>(onMessageReceived);

	std::cout << "Starting Synaps connection" << std::endl;
	serverConnection.start();
	internalMessageTool.start();

	while (serverConnection.alive()) {}
}

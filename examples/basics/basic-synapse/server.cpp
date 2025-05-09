/*
* This example shows basic connection, using Axon internal tools
* Synapse provides connection between two points
* 
* This file defines basic server logic, with message handling using Synapse event system
*/

#include <networking/AxonNetwork.hpp>
#include <iostream>

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
{
	std::cout << reinterpret_cast<char*>(event.getMessage().getMessage()) << std::endl;
}

int main()
{
	Networking::AsyncSynapse serverConnection = Networking::AsyncSynapse<Networking::ConnectionMode::TCP>(10423);
	serverConnection.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);

	std::cout << "Starting Synapse connection" << std::endl;
	serverConnection.start();

	while (serverConnection.alive()) {}
}

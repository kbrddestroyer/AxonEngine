/*
* This example shows basic server, using Axon internal tools.
* Synapse provides connection between two points, this one is created in server mode.
* 
* This file shows basic server logic and message handling using asynchronous Synapse with event system.
*/

#include <networking/AxonNetwork.hpp>
#include <iostream>
#include <iomanip>

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
{
    if (!event.getMessage().getMessage())
    {
        std::cout << "PING! --> " << std::endl <<
                event.getMessage().ID() << " " <<
                event.getMessage().getSize() << " " <<
                event.getMessage().getFlagSet() << " " <<
                static_cast<uint16_t>(event.getMessage().getPartID()) << std::endl;
        return;
    }

    std::cout << "BODY: " << std::endl <<
              static_cast<char*>(event.getMessage().getMessage()) << " " <<
              event.getMessage().ID() << " " <<
              event.getMessage().getSize() << " " <<
              event.getMessage().getFlagSet()
              << std::endl;
}

int main()
{
	Networking::Synapse<
	        Networking::ConnectionMode::TCP,
            Networking::SynapseMode::SERVER,
            Networking::BerkeleyAxonNetworkController<Networking::ConnectionMode::TCP, Networking::SynapseMode::SERVER>
            > serverConnection(10423);
	serverConnection.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);

	std::cout << "Starting Synapse connection" << std::endl;
	serverConnection.start();

	while (serverConnection.alive()) {}
}

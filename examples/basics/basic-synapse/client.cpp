/*
* This example shows basic connection, using Axon internal tools
* Synapse provides connection between two points
* 
* This file defined basic client logic, with AxonMessage forming and sending with Synapse tool
*/

#include <networking/AxonNetwork.hpp>
#include <cstring>
#include <iostream>

#include <chrono>
#include <sstream>

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
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::AsyncSynapse<Networking::ConnectionMode::TCP, Networking::SynapseMode::CLIENT> clientConnection(connection);
    clientConnection.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    clientConnection.start();

    time_t startTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    Networking::AxonMessage msg(nullptr, 0);
    msg.addFlag(Networking::VALIDATE);
    clientConnection.send(msg);

    for (uint8_t i = 0; i < 3; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::stringstream sstream;

        sstream <<
            "Sending message on " <<
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - startTimestamp;

        std::cout << "Sending: " << sstream.str() << std::endl;

        Networking::AxonMessage message_(const_cast<char*>(sstream.str().c_str()), sstream.str().length() + 2, 0, Networking::TAG_FLAGS::VALIDATE);

        clientConnection.send(message_);
    }
}

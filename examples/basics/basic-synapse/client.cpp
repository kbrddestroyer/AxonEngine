/*
* This example shows basic client connection, using Axon internal tools.
* Synapse provides connection between two points, this one is created in client mode.
* 
* This file defined basic client logic. It creates Message package and sends it to server. Also, it is capable of
* server messages handling, displaying response packages.
*/

#include <networking/AxonLibrary.hpp>
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
	Networking::AsyncSynapse<
            Networking::BerkeleyAxonNetworkController<Networking::TCP, Networking::SynapseMode::CLIENT>
            > clientConnection("localhost", 10423);
    clientConnection.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
    clientConnection.start();

    time_t startTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    Networking::AxonMessage msg(nullptr, 0);
    msg.addFlag(Networking::VALIDATE);
    clientConnection.send(msg);

    for (uint8_t i = 0; i < 3; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::stringstream stream;

        stream <<
            "Sending message on " <<
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - startTimestamp;

        std::cout << "Sending: " << stream.str() << std::endl;

        Networking::AxonMessage message_(const_cast<char*>(stream.str().c_str()), stream.str().length() + 2, 0, Networking::TAG_FLAGS::VALIDATE);

        clientConnection.send(message_);
    }
}

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


int main()
{
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::Synapse<Networking::ConnectionMode::UDP, Networking::SynapseMode::CLIENT> clientConnection(connection);

    time_t startTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    for (uint8_t i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::stringstream sstream;

        sstream <<
            "Sending message on " <<
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - startTimestamp;
        Networking::AxonMessage message_(const_cast<char*>(sstream.str().c_str()), sstream.str().length() + 1);

        clientConnection.send(message_);
    }
}

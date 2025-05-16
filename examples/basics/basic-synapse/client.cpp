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

	Networking::AsyncSynapse<Networking::ConnectionMode::UDP> clientConnection(connection);

	clientConnection.start();

    time_t startTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::stringstream sstream;

        sstream <<
            "Sending message on " <<
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - startTimestamp;
        Networking::AxonMessage message_(const_cast<char*>(sstream.str().c_str()), sstream.str().length());

        clientConnection.sendPooled(message_);
    }
}

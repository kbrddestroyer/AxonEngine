/*
* This example shows basic connection, using Axon internal tools
* Synapse provides connection between two points
* 
* This file defined basic client logic, with AxonMessage forming and sending with Synapse tool
*/

#include <networking/AxonNetwork.hpp>
#include <cstring>
#include <iostream>


int main()
{
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::AsyncSynapse<Networking::ConnectionMode::UDP> clientConnection(connection);
	const char* message = "Hello World!";

	clientConnection.start();

	Networking::AxonMessage message_(const_cast<char*>( message ), strlen(message) + 1, 1);
	clientConnection.sendPooled(message_);
}

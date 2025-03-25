/*
* This example shows basic connection, using Axon internal tools
* Synapse provides connection between two points
* 
* This file defined basic client logic, with AxonMessage forming and sending with Synapse tool
*/

#include <networking/AxonNetwork.hpp>
#include <string.h>


int main()
{
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::Synapse clientConnection = Networking::Synapse(connection);

	const char* message = "Hello World!";
	Networking::AxonMessage message_ = Networking::AxonMessage(message, strlen(message) + 1);
	clientConnection.send(message_);
}
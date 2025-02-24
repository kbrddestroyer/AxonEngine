/*
* This example shows basic connection, using Axon internal tools
* Synaps provides connection between two points
* 
* This file defined basic client logic, with AxonMessage forming and sending with Synaps tool
*/

#include <networking/AxonNetwork.hpp>
#include <string.h>


int main()
{
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::Synaps clientConnection = Networking::Synaps(connection);

	const char* message = "Hello World!";
	Networking::AxonMessage message_ = Networking::AxonMessage(message, strlen(message) + 1);
	clientConnection.send(message_);
}
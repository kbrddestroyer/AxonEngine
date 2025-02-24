#include <networking/AxonNetwork.hpp>
#include <string.h>


int main()
{
	Networking::ConnectionInfo connection = { "localhost", 10423 };

	Networking::Synaps clientConnection = Networking::Synaps(connection);

	const char* message = "Hello World!";
	clientConnection.send(Networking::AxonMessage(message, strlen(message) + 1));
}
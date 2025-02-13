/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backend.h>
#include <string.h>


int main()
{
	SOCKADDR_IN_T socket;
	SOCKET_T client;
	const char* hostname = "localhost";
	uint32_t port = 10423;

	std::cout << "Starting client socket initialization..." << std::endl;
	std::cout << "HOSTNAME: " << hostname << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = connect_udp_client(&socket, &client, hostname, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;

	const uint8_t messagesCount = 2;

	const char* messages[messagesCount] = {
		"Hello from client example!",
		"q!"
	};

	for (uint8_t i = 0; i < messagesCount; i++)
		if (int code = send_udp_message(messages[i], strlen(messages[i]), client, &socket) < 0)
		{
			return GET_SOCKET_ERROR();
		}

	std::cin.get();

	finalize_udp(&client);
	return 0;
}

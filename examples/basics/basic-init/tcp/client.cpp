/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>
#include <string.h>


int main()
{
	SOCKADDR_IN_T socket;
	SOCKET_T client;
	const char* hostname = "127.0.0.1";
	uint32_t port = 10423;

	std::cout << "Starting client socket initialization..." << std::endl;
	std::cout << "HOSTNAME: " << hostname << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";
	int code = connect_tcp_client(&socket, &client, hostname, port);
	if (code != SUCCESS)
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
		if (int code = send_tcp_message(messages[i], strlen(messages[i]), client) < 0)
		{
			return GET_SOCKET_ERROR();
		}

	std::cin.get();

	finalize_tcp(client);
	return 0;
}

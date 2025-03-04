/*
*	Low-level showcase of client socket initialization
*/

#include <iostream>
#include <backends/backend.h>
#include <thread>
#include <string.h>

int main()
{
	SOCKADDR_IN_T server;
	SOCKET_T server_socket;
	uint32_t port = 10423;

	std::cout << "Starting server socket initialization..." << std::endl;
	std::cout << "PORT:		" << port << std::endl;

	std::cout << "Initializing...";

	if (int code = create_tcp_server(&server, &server_socket, port) != SUCCESS)
	{
		std::cout << "Initialization failed with code " << code << std::endl;
		return code;
	}

	std::cout << "SUCCESS!" << std::endl;

	char			message_buffer[256] = {};
	uint8_t			packages = 0;
	SOCKADDR_IN_T	from;

	SOCKET_T client = accept_incoming(server_socket, &from);

	while (true)
	{
		if (int code = recv_tcp_message((char**) message_buffer, 256, client, &from) > 0)
		{
			std::cout << message_buffer << std::endl;
			packages++;
			if (strcmp(message_buffer, "q!"))
				break;
		}
		else
		{
			if (int lastError = GET_SOCKET_ERROR() > 0)
			{
				std::cout << lastError << std::endl;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "FINISH!" << std::endl;

	finalize_tcp(server_socket);

	std::cin.get();
	return 0;
}

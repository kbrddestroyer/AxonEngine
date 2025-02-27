#include <iostream>
#include <chrono>

#include <AxonNetwork.hpp>
#include <thread>

#include "common.h"

void onMessageRecieved(Networking::SynapsMessageReceivedEvent* event)
{
	const time_t serverTime = *(reinterpret_cast<const time_t*> (event->getMessage().getMessage()));
	std::cout << "Ping is " << serverTime << std::endl;
}

void ping(Networking::AsyncSynaps& synaps)
{
	std::chrono::system_clock::duration currentTime = std::chrono::system_clock::now().time_since_epoch();
	time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

	char* buffer = new char[sizeof(time_t)];
	memcpy(buffer, &timestamp, sizeof(time_t));

	Networking::AxonMessage message = Networking::AxonMessage(buffer, sizeof(time_t));
	synaps.send(message);
}

int main()
{
	Networking::AsyncSynaps connection({ "localhost", 10423 });
	connection.getEventManager().subscribe<Networking::SynapsMessageReceivedEvent>(onMessageRecieved);
	connection.start();
	for (uint8_t i = 0; i < 8; i++)
	{
		ping(connection);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
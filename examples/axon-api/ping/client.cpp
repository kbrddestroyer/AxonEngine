#include <iostream>
#include <chrono>

#include <AxonNetwork.hpp>
#include <thread>

#include "common.h"

void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
{
	const time_t serverTime = *(reinterpret_cast<const time_t*> (event.getMessage().getMessage()));
	std::cout << "Ping is " << serverTime << std::endl;
}

void ping(Networking::AsyncSynapse& synapse)
{
	std::chrono::system_clock::duration currentTime = std::chrono::system_clock::now().time_since_epoch();
	time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();
	std::cout << "sent " << timestamp << std::endl;
	
	Networking::AxonMessage message = Networking::AxonMessage(&timestamp, sizeof(time_t));
	synapse.send(message);
}

int main()
{
	Networking::AsyncSynapse connection({ "localhost", 10423 });
    connection.getEventManager().subscribe<Networking::SynapseMessageReceivedEvent>(onMessageReceived);
	connection.start();
	for (uint8_t i = 0; i < 8; i++)
	{
		ping(connection);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
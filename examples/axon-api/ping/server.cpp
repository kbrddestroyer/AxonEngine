#include <iostream>
#include <chrono>

#include <AxonNetwork.hpp>
#include "common.h"


struct SynapseHolder
{
	Networking::Synapse& synapse;

	void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
	{
		std::chrono::system_clock::duration currentTime = std::chrono::system_clock::now().time_since_epoch();
		time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

		const time_t clientTime = *(reinterpret_cast<const time_t*> (event.getMessage().getMessage()));

		time_t ping = timestamp - clientTime;
		std::cout << "ping is " << ping << std::endl;
		Networking::AxonMessage message = Networking::AxonMessage(&ping, sizeof(time_t));
		synapse.sendTo(message, event.getFrom());
	}
};

int main()
{
	Networking::Synapse synapse(10423, Networking::ConnectionMode::UDP);
	SynapseHolder holder = {synapse };

	synapse.getEventManager().subscribe<SynapseHolder, Networking::SynapseMessageReceivedEvent>(&SynapseHolder::onMessageReceived, &holder);
	synapse.start();
}
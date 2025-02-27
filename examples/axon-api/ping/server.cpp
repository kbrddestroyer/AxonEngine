#include <iostream>
#include <chrono>

#include <AxonNetwork.hpp>
#include "common.h"


Networking::Synaps* g_synaps;	// Never do this pls


void onMessageReceived(Networking::SynapsMessageReceivedEvent* event)
{
	if (!g_synaps)
		return;

	std::chrono::system_clock::duration currentTime = std::chrono::system_clock::now().time_since_epoch();
	time_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();

	const time_t clientTime = *(reinterpret_cast<const time_t*> (event->getMessage().getMessage()));

	time_t ping = timestamp - clientTime;
	char* buffer = new char[sizeof(time_t)];
	memcpy(buffer, &ping, sizeof(time_t));

	Networking::AxonMessage message = Networking::AxonMessage(buffer, sizeof(time_t));
	g_synaps->sendTo(message, event->getFrom());

	delete[] buffer;
}


int main()
{
	Networking::Synaps synaps(10423);
	g_synaps = &synaps;

	synaps.getEventManager().subscribe<Networking::SynapsMessageReceivedEvent>(onMessageReceived);

	synaps.start();
}
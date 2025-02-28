#include <iostream>
#include <vector>
#include <AxonNetwork.hpp>


class CentralSynaps : public Networking::Synaps
{
	std::vector<SOCKADDR_IN_T> clients;

public:
	CentralSynaps(uint32_t port) : Networking::Synaps(port) {}

	void onMessageReceived(Networking::SynapsMessageReceivedEvent* event)
	{
		Networking::AxonMessage serialized = event->getMessage();
		Networking::Message message(serialized);

		if (message.getTag() != 0)
		{
			this->sendTo(serialized, event->getFrom());
			clients.push_back(*(event->getFrom()));
			return;
		}

		for (SOCKADDR_IN_T& to : clients)
		{
			this->sendTo(serialized, &to);
		}
	}

	void start() override
	{
		this->getEventManager().subscribe<CentralSynaps, Networking::SynapsMessageReceivedEvent>(&CentralSynaps::onMessageReceived, this);
		Networking::Synaps::start();
	}
};


int main()
{
	CentralSynaps central(10423);

	central.start();
}
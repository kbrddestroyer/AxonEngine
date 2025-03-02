#include <iostream>
#include <vector>
#include <AxonNetwork.hpp>


class CentralSynaps : public Networking::Synaps
{
	inline static uint32_t ID = 0;

	struct ClientConnection
	{
		uint32_t id;
		SOCKADDR_IN_T connection;
	};

	std::vector<ClientConnection> clients;

public:
	CentralSynaps(uint32_t port) : Networking::Synaps(port) {}

	void onMessageReceived(const Networking::SynapsMessageReceivedEvent& event)
	{
		Networking::AxonMessage serialized = event.getMessage();

		if (serialized.getTag() != 0)
		{
			this->sendTo(serialized, event.getFrom());
			clients.push_back({ ++CentralSynaps::ID, *(event.getFrom())} );
			return;
		}

		for (const ClientConnection& to : clients)
		{
			this->sendTo(serialized, &to.connection);
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
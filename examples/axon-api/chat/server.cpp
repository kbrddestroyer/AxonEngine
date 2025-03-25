#include <iostream>
#include <vector>
#include <AxonNetwork.hpp>


class CentralSynapse : public Networking::Synapse
{
	inline static uint32_t ID = 0;

	struct ClientConnection
	{
		uint32_t id;
		SOCKADDR_IN_T connection;
	};

	std::vector<ClientConnection> clients;

public:
	CentralSynapse(uint32_t port) : Networking::Synapse(port) {}

	void onMessageReceived(const Networking::SynapseMessageReceivedEvent& event)
	{
		Networking::AxonMessage serialized = event.getMessage();

		if (serialized.getTag() != 0)
		{
			this->sendTo(serialized, event.getFrom());
			clients.push_back({ ++CentralSynapse::ID, *(event.getFrom())} );
			return;
		}

		for (const ClientConnection& to : clients)
		{
			this->sendTo(serialized, &to.connection);
		}
	}

	void start() override
	{
		this->getEventManager().subscribe<CentralSynapse, Networking::SynapseMessageReceivedEvent>(&CentralSynapse::onMessageReceived, this);
		Networking::Synapse::start();
	}
};


int main()
{
	CentralSynapse central(10423);

	central.start();
}
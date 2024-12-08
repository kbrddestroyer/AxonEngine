#pragma once

#include <Axon.h>
#include <client/client_connection.h>


#define G_CLIENT Axon::Client::CLIENT()


namespace Axon
{
	class AXON_DECLSPEC Client
	{
	private:
		Axon::Connection::ClientConnectionHandler* handler;
	private:
		Client() = default;
	public:
		Client(char*, Axon::Connection::AXON_PORT);
		~Client();

		void SendUserMessage(Axon::Connection::Message& message);
	};

	class ClientSingleton
	{
	private:
		inline static Client* instance = nullptr;
		inline static uint32_t refcnt = 0;
	public:
		inline ClientSingleton(char* hostname, Axon::Connection::AXON_PORT port)
		{
			if (!instance)
				instance = new Client(hostname, port);

			refcnt++;
		}

		inline ~ClientSingleton()
		{
			refcnt--;
			if (refcnt == 0)
				delete instance;
		}

		inline static Client* get_client()
		{
			return instance;
		}

		inline Axon::Client* operator*() { return instance; }
		inline Axon::Client* operator->() { return instance; }
	};
}
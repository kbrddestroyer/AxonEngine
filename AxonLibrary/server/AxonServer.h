#pragma once
#include <server/server_connection.h>

#include <vector>


namespace Axon
{
	AXON_DECLSPEC typedef void(*SERVER_CALLBACK_FUNC) (Axon::Connection::ServerUDPMessage msg);

	class AXON_DECLSPEC Server
	{
	private:
		Axon::Connection::ServerConnectionHandler* handler;
		std::vector<SERVER_CALLBACK_FUNC> calls;
	public:
		Server();
		~Server();

		void Start();
		void Subscribe(SERVER_CALLBACK_FUNC callback);
	private:
		void onMessageRecived(Axon::Connection::ServerUDPMessage);

		static void onMessageRecivedCall(Axon::Connection::ServerUDPMessage, void*);
	};
}
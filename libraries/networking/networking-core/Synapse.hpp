#pragma once
#include <thread>
#include <atomic>
#include <string>

#include <AxonUtility.h>
#include <events/AxonEvent.hpp>
#include <backends/backend.hpp>
#include <networking/message/AxonMessage.hpp>

namespace Networking
{
	const uint32_t SYNAPSE_MESSAGE_MAX_SIZE = 1024;

	enum ConnectionMode
	{
		UDP = SOCK_DGRAM,
		TCP = SOCK_STREAM
	};

	/**
	* Low-level connection info for convenient storage
	* TODO:
	* - Research replacement
	*/
	struct ConnectionInfo
	{
		std::string	hostname = "";
		uint32_t	port;
	};

	/**
	* TODO: Documenting
    */
    class AxonNetworkingInternalError : public std::exception
	{
		uint8_t err;
	public:
		inline explicit AxonNetworkingInternalError(uint8_t err = 0) : err(err) {}
		inline constexpr uint8_t code() const { return err; }
	};

	/**
	* Synapse message when message is recieved from remote host
	*
	* Functions:
	* getMessage() -> AxonMessage: get AxonMessage object
	* getFrom() -> SOCKADDR_IN_T: get sender network info
	*/
	class AXON_DECLSPEC SynapseMessageReceivedEvent final : public EventSystem::AxonEvent
	{
	private:
		const AxonMessage&		message;
		SOCKADDR_IN_T* from;
	public:
		SynapseMessageReceivedEvent(const AxonMessage& message, SOCKADDR_IN_T* from) : EventSystem::AxonEvent(), message(message)
		{
			this->from = from;
		}

		inline const AxonMessage& getMessage() const { return message; }
		inline SOCKADDR_IN_T* getFrom() const { return from; }
	};

	/**
	* Axon connection handler
	* Specifies connection handling to one remote host
	*
	* EventSystem API:
	* WIP currently
	*
	* TODO:
	*	- Documenting
	*/
	
	template <ConnectionMode mode> class AXON_DECLSPEC Synapse
	{
	private:
		EventSystem::AxonEventManager events;

		bool				isServer;
		ConnectionInfo		info;
		Socket              socket;
	protected:
		std::atomic<bool>	isAlive = false;
	public:
		/** Default creation is restriced */
		Synapse() = delete;

		/** Initializes Synapse in server mode */
		explicit Synapse(uint32_t);
		/** Initialize Synapse in client mode */
		explicit Synapse(const ConnectionInfo&);
		virtual ~Synapse();
    private:
        void initializeFromConnectionMode();
	public:
		inline bool alive() const { return isAlive.load(); };

		virtual void start();

		void send(const AxonMessage&);
		void sendTo(const AxonMessage&, const SOCKADDR_IN_T*) const;

		void listen();

		virtual void onMessageReceived(const AxonMessage&, SOCKADDR_IN_T*);

		constexpr inline EventSystem::AxonEventManager& getEventManager() { return events; }
	};

	template <ConnectionMode mode>
	class AXON_DECLSPEC AsyncSynapse : public Synapse<mode>
	{
	private:
		std::thread proc;
        bool isAlive;
	public:
		/** Initializes Synapse in server mode */
		inline AsyncSynapse<mode>(uint32_t port) : Synapse<mode>(port) {}
		/** Initialize Synapse in client mode */
		inline AsyncSynapse<mode>(const ConnectionInfo& info) : Synapse<mode>(info) {}
		
		virtual ~AsyncSynapse();

		void start() override;
		void kill();
	};

    extern template class Synapse<ConnectionMode::UDP>;
    extern template class Synapse<ConnectionMode::TCP>;
    extern template class AsyncSynapse<ConnectionMode::UDP>;
    extern template class AsyncSynapse<ConnectionMode::TCP>;
}

/* Synapse.hpp */

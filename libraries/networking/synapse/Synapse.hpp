#pragma once
#include "BasicSynapse.hpp"
#include "networking/utility/MessagePool.hpp"

#include <thread>
#include "AxonUtility.h"
#include "messages/AxonMessage.hpp"

#include "networking/synapse/utils/SynapseUtility.hpp"

namespace Networking
{
	/**
	 *	Advanced connection handler with event system
	 *
     * @tparam controller derived from AxonNetworkControllerBase class
	 */
    template<class controller>
	class AXON_DECLSPEC Synapse : public BasicSynapse<controller> {
	public:
#pragma region CONSTRUCTING
		/** Initializes Synapse in server mode */
		explicit Synapse(uint32_t port) : BasicSynapse<controller>(port) {}
		/** Initialize Synapse in client mode */
		explicit Synapse(const ConnectionInfo &info) : BasicSynapse<controller>(info) {}

		~Synapse() override = default;
#pragma endregion

#pragma region INTERFACE
		void update() override;
		void onMessageReceived(const AxonMessage&, const Socket&) override;

        void sendTo(AxonMessage&, const Socket&) override;
		void sendPooled(const AxonMessage&, const Socket &) const;

        GETTER bool alive() { return this->networkController->isAlive(); }
#pragma endregion
	protected:
        std::vector<uint64_t> pendingValidation;
		std::unique_ptr<MessagePoolBase> pool = std::make_unique<MessagePoolBase>();
		std::unique_ptr<MessageMapBase> mmap = std::make_unique<MessageMapBase>();
	};

    /**
     *	Synapse with listen() function in separated thread.
     *
     * @tparam controller derived from AxonNetworkControllerBase class
     */
	template <class controller>
	class AXON_DECLSPEC AsyncSynapse final : public Synapse<controller>
	{
	public:
		/** Initializes Synapse in server mode */
		explicit AsyncSynapse(uint32_t port) : Synapse<controller>(port) {}

		/** Initialize Synapse in client mode */
		explicit AsyncSynapse(const ConnectionInfo &info) : Synapse<controller>(info) {}

		~AsyncSynapse() override;

		void start() override;
		void kill() override;
	private:
		std::thread proc;
	};
}

#include "Synapse.ipp"

/* Synapse.hpp */

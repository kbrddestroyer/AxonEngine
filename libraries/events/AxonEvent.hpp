#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>


namespace EventSystem
{
	/**
	 * Base class for any AxonEvent. Provides convenient interface (or will do some day).
	 */
	class AxonEvent {
	public:
		virtual ~AxonEvent() = default;
	};


	/**
	 * EventManager class is capable of dispatching triggered events.\n
	 * <b> Usage: </b>
	 * EventManager could be created literally anywhere.
	 * Callback method can be attached with <b>subscribe()</b> method. It will be dispatched when
	 * <b>invoke()</b> message is called.
	 *
	 * @note see GlobalEventManager
	 * @note see client-server example
	 */
	class AxonEventManager
	{
	public:
		AxonEventManager() = default;
		virtual ~AxonEventManager();

		template<typename T>
		void subscribe(std::function<void(const T&)>);
		template<class C, typename T>
		void subscribe(void (C::* callback)(const T&), C *instance);
		
		void invoke(AxonEvent *);
	private:
		std::unordered_map<std::type_index, std::vector<std::function<void(AxonEvent*)>>> subscribers;
	};

	/**
	 * GlobalEventManager cannot be constructed, only accessed via Instance() method
	 */
	class GlobalEventManager final : public AxonEventManager
	{
		GlobalEventManager() = default;
	public:
		static GlobalEventManager& Instance() noexcept;
	};
}

#include "AxonEvent.ipp"
/* AxonEvent.hpp */

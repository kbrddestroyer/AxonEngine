#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include <cstdint>

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
	 * <br>
	 * EventManager could be created literally anywhere.
	 * Callback method can be attached with <b>subscribe()</b> method. It will be dispatched when
	 * <b>invoke()</b> message is called.
	 * <br>
	 * <br>
	 * <b> IMPORTANT NOTE: </b>
	 * <br>
	 * Please, make sure that you unsubscribe class method in destructor. EventManager contains class method reference and will try to call it, even if object is nonexistent. This is undefined behaviour and most likely will cause a segmentation fault on Unix systems and SEH exception on Windows.
	 *
	 * @note see GlobalEventManager
	 * @note see client-server example
	 */
	class AxonEventManager
	{
    protected:
        typedef uint64_t desc_t;
        typedef std::type_index primary_key_t;
        typedef std::function<void(AxonEvent*)> event_call_t;
        typedef std::unordered_map<desc_t, event_call_t> calls_c;
	public:
		AxonEventManager() = default;
		virtual ~AxonEventManager();

		template<typename T>
        void subscribe(std::function<void(const T&)>);
		template<class C, typename T>
		void subscribe(void (C::* callback)(const T&), C *instance);

        template<typename T>
        void unsubscribe(std::function<void(const T&)>);

        template<class C, typename T>
        void unsubscribe(void (C::* callback)(const T&), C *instance);

		void invoke(AxonEvent *);
    protected:
        template <typename T>
        desc_t generateDescriptor(std::function<void(const T&)>);

        template <class C, typename T>
        desc_t generateDescriptor(void (C::* callback)(const T&), C *instance);
	private:
		std::unordered_map<primary_key_t, calls_c> subscribers;
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

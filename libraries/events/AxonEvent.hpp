#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <cstdint>


namespace EventSystem
{
	class AxonEvent {
	public:
		virtual ~AxonEvent() = default;
 	};

	class AxonEventManager
	{
	public:
		AxonEventManager() = default;
		virtual ~AxonEventManager();

		template<typename T>
		void subscribe(std::function<void(const T&)>);

		template<class C, typename T>
		void subscribe(void (C::* method)(const T&), C* instance);
		
		void invoke(AxonEvent*);
	private:
		std::unordered_map<std::type_index, std::vector<std::function<void(AxonEvent*)>>> subscribers;
	
		friend class GlobalEventManager;
	};

	class GlobalEventManager : public AxonEventManager
	{
	private:
		GlobalEventManager() = default;
	public:
		static GlobalEventManager& Instance();
	};
}

#include "AxonEvent.ipp"
/* AxonEvent.hpp */

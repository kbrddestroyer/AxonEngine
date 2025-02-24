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
		AxonEventManager();
		~AxonEventManager();

		template<typename T>
		inline void subscribe(std::function<void(T*)>);
		
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
		static GlobalEventManager* Instance();
	private:
		static GlobalEventManager* instance;
	};
}

#include "AxonEvent.ipp"
/* AxonEvent.hpp */

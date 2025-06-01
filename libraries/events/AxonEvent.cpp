#include "AxonEvent.hpp"


EventSystem::AxonEventManager::~AxonEventManager()
{
	for (std::pair<std::type_index, std::vector<std::function<void(AxonEvent*)>>> calls : subscribers)
	{
		calls.second.clear();
	}
	subscribers.clear();
}

void EventSystem::AxonEventManager::invoke(AxonEvent* event)
{
	std::type_index type = std::type_index(typeid(*event));
	std::vector<std::function<void(AxonEvent*)>> calls = subscribers[type];

	for (const std::function<void(AxonEvent*)>& callback : calls)
	{
		callback(event);
	}
}

EventSystem::GlobalEventManager& EventSystem::GlobalEventManager::Instance()
{
    static EventSystem::GlobalEventManager instance;
	return instance;
}

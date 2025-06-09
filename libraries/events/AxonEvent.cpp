#include "AxonEvent.hpp"


EventSystem::AxonEventManager::~AxonEventManager()
{
	for (auto & calls : subscribers)
	{
		calls.second.clear();
	}
	subscribers.clear();
}

void EventSystem::AxonEventManager::invoke(AxonEvent* event)
{
	const auto type = std::type_index(typeid(*event));
	calls_c calls = subscribers[type];

	for (const auto & callback : calls)
	{
		callback.second(event);
	}
}

EventSystem::GlobalEventManager& EventSystem::GlobalEventManager::Instance() noexcept
{
    static GlobalEventManager instance;
	return instance;
}

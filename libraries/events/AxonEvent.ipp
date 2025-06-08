#pragma once

template<typename T>
void EventSystem::AxonEventManager::subscribe(std::function<void(const T&)> callback)
{
	std::type_index type = std::type_index(typeid(T));

	subscribers[type].push_back(
		[callback](AxonEvent* base_event) {
			if (T* event = dynamic_cast<T*>(base_event))
			{
				callback(*event);
			}
		}
	);
}

template<class C, typename T>
void EventSystem::AxonEventManager::subscribe(void(C::* callback)(const T&), C* instance)
{
	const auto type = std::type_index(typeid(T));
	subscribers[type].push_back(
		[callback, instance](AxonEvent* base_event) {
			if (T* event = dynamic_cast<T*>(base_event))
			{
				(instance->*callback)(*event);
			}
		}
	);
}

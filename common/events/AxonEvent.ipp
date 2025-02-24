#pragma once

template<typename T>
inline void EventSystem::AxonEventManager::subscribe(std::function<void(T*)> callback)
{
	std::type_index type = std::type_index(typeid(T));

	subscribers[type].push_back(
		[callback](AxonEvent* base_event) {
			if (T* event = dynamic_cast<T*>(base_event))
			{
				callback(event);
			}
		}
	);
}

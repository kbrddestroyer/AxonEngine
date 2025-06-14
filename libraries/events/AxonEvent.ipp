#pragma once

template<typename T>
void EventSystem::AxonEventManager::subscribe(std::function<void(const T&)> callback)
{
	const auto type = std::type_index(typeid(T));

    desc_t desc = generateDescriptor(callback);

	subscribers[type][desc] = (
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
    desc_t desc = generateDescriptor(callback, instance);

	subscribers[type][desc] = (
		[callback, instance](AxonEvent* base_event) {
			if (T* event = dynamic_cast<T*>(base_event))
			{
				(instance->*callback)(*event);
			}
		}
	);
}

template<typename T>
void EventSystem::AxonEventManager::unsubscribe(std::function<void(const T &)> callback) {
    const auto type = std::type_index(typeid(T));
    desc_t desc = generateDescriptor(callback);

    subscribers[type].erase(desc);
}

template<class C, typename T>
void EventSystem::AxonEventManager::unsubscribe(void (C::* callback)(const T&), C *instance) {
    const auto type = std::type_index(typeid(T));
    desc_t desc = generateDescriptor(callback, instance);

    subscribers[type].erase(desc);
}

template<typename T>
uint64_t EventSystem::AxonEventManager::generateDescriptor(std::function<void(const T &)> callback) {
    const auto fPtr = callback.template target<void(const T &)>();
    return std::hash<void*>() (*(void**) &fPtr);
}

template<class C, typename T>
uint64_t EventSystem::AxonEventManager::generateDescriptor(void (C::* callback)(const T&), C *instance) {
    return std::hash<C *>() (instance);
}

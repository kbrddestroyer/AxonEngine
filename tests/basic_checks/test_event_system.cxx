#include <events/AxonEvent.hpp>

#include <cassert>
#include <cstdint>


class TestEvent : public EventSystem::AxonEvent
{
private:
	uint8_t data;
	bool isChecked = false;
public:
	TestEvent(uint8_t data = 0)
	{
		this->data = data;
	}

	constexpr uint8_t get_data() const { return data; }
};


void callback(const TestEvent& event)
{
	uint8_t data = event.get_data();
	assert(data == 3);
}

void callback2(const TestEvent& event)
{
	uint8_t data = event.get_data();
	assert(data == 3);
}

int main()
{
	/* Local events */
	TestEvent event(3);

	EventSystem::AxonEventManager manager;

	manager.subscribe<TestEvent>(callback);
	manager.invoke(&event);

	/* Global event manager */
	EventSystem::GlobalEventManager* g_manager = EventSystem::GlobalEventManager::Instance();
	g_manager->subscribe<TestEvent>(callback);
	g_manager->subscribe<TestEvent>(callback2);

	g_manager->invoke(&event);
}

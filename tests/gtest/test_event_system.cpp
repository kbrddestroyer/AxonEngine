#include <gtest/gtest.h>

#include <networking/AxonLibrary.hpp>

class TestEvent : public EventSystem::AxonEvent
{
public:
    TestEvent() = delete;

    explicit TestEvent(uint8_t data = 0)
    {
        this->data = data;
    }

    uint8_t get_data() const { return data; }
private:
    uint8_t data;
    bool isChecked = false;
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

TEST(TEST_EVENT_SYSTEM, TEST_EVENT_SYSTEM_GENERAL)
{
    /* Local events */
    TestEvent event(3);

    EventSystem::AxonEventManager manager;

    manager.subscribe<TestEvent>(callback);
    manager.invoke(&event);

    /* Global event manager */
    EventSystem::GlobalEventManager& g_manager = EventSystem::GlobalEventManager::Instance();
    g_manager.subscribe<TestEvent>(callback);
    g_manager.subscribe<TestEvent>(callback2);

    g_manager.invoke(&event);
}

TEST(TEST_EVENT_SYSTEM, TEST_EVENT_SYSTEM_W_CLASS_OBJ)
{
    /* Local events */
    TestEvent event(3);
    EventSystem::GlobalEventManager& g_manager = EventSystem::GlobalEventManager::Instance();

    class TestEventCheckClass {
    public:
        TestEventCheckClass() {
            EventSystem::GlobalEventManager::Instance().subscribe<
                TestEventCheckClass, TestEvent
                >(&TestEventCheckClass::callback, this);
        }

        void callback(const TestEvent &) {
            call = true;
        }

        bool check() const { return call; }
    private:
        bool call = false;
    } instance;

    g_manager.invoke(&event);
    ASSERT_TRUE(instance.check());
}

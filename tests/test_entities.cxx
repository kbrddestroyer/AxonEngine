#include <AxonEngine.h>
#include <AxonEntity.h>


class TestEntity : public Axon::EntityBase
{
public:
	std::string toString() override { return "test entity"; }
	void fromString(std::string initData) override {}
};


int main()
{
	// 1. Create new entity via macro and check g_inCreation

	std::weak_ptr<TestEntity> entity = CREATE_ENTITY(TestEntity);

	_ASSERT(Axon::EntityBase::IN_CREATION().size() == 1);

	entity.lock()->onCreated();
	Axon::ENTITY_MAP::iterator res = Axon::EntityBase::ENTITIES().find(entity.lock()->ID());
	Axon::ENTITY_MAP::iterator end = Axon::EntityBase::ENTITIES().end();
	if (res == end)
		return 1;
	return 0;
}

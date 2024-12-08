#ifndef AXON_CLIENT
#define AXON_CLIENT
#endif

#include <AxonEngine.h>
#include <AxonEntity.h>

#include <iostream>


class TestEntity : public Axon::EntityBase
{
public:
	uint32_t data = 0;
public:
	std::string toString() { 
		return std::string((const char*) data);
	}

	void fromString(std::string initData) {
		data = *reinterpret_cast<uint32_t*>((char*) initData.c_str());
	}
};


int main()
{
	Axon::ClientSingleton client = Axon::ClientSingleton((char*)"localhost", 10243);

	const char* message_data = "Hello from Axon client";

	Axon::Connection::Message message = Axon::Connection::Message::createUDPMessage((void*)message_data, strlen(message_data), 1);

	for (uint16_t index = 0; index < 8; index++)
	{
		client->SendUserMessage(message);
	}

	std::weak_ptr<TestEntity> entity = CREATE_ENTITY(TestEntity);
	
	std::cin.get();
	return 0;
}

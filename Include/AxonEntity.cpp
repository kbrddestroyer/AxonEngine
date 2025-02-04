#include "AxonEntity.h"
#include <AxonEngine.h>

#include <iostream>


Axon::EntityBase::EntityBase()
{
	entityID = 0;
}

Axon::EntityBase::~EntityBase()
{

}

void Axon::EntityBase::dispatchCreationMessage()
{
	Axon::Connection::Message message = Axon::Connection::Message::createUDPMessage((void*) toString().c_str(), toString().length(), ID());

#if defined(AXON_SERVER)

	// One to many
#elif defined(AXON_CLIENT)
	std::cout << "Client -> Server data transmittion" << std::endl;
	Axon::ClientSingleton::get_client()->SendUserMessage(message);
#else
#pragma warning("Compiling executable without AXON_[CLIENT|SERVER] macro. There will be no exec mode on target")
#endif
}

void Axon::EntityBase::update()
{
	Axon::Connection::Message message = Axon::Connection::Message::createUDPMessage((void*)toString().c_str(), toString().length(), ID());

#if defined(AXON_SERVER)

	// One to many

#elif defined(AXON_CLIENT)
	std::cout << "Client -> Server data transmittion" << std::endl;
	Axon::ClientSingleton::get_client()->SendUserMessage(message);
#else
#pragma warning("Compiling executable without AXON_[CLIENT|SERVER] macro. There will be no exec mode on target")
#endif
}

void Axon::EntityBase::onControlMessageRecived()
{

}

void Axon::EntityBase::onCreated()
{
	std::shared_ptr<Axon::EntityBase> s_ptr;

	for (
		ENTITY_POOL::iterator iter = Axon::EntityBase::g_inCreation.begin();
		iter < Axon::EntityBase::g_inCreation.end();
		iter++
		)
		if (iter->get() == this)
		{
			s_ptr = *iter;
			Axon::EntityBase::g_inCreation.erase(iter);
			break;
		}

	Axon::EntityBase::g_entities[ID()] = s_ptr;
}

void Axon::EntityBase::onInitDataRecived()
{

}

void Axon::EntityBase::onDestroyed()
{
	Axon::EntityBase::g_entities.erase(this->entityID);
}

void Axon::EntityBase::sendID()
{

}

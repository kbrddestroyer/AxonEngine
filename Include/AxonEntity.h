#pragma once

#include <AxonEngine.h>

#include <cstdint>
#include <map>
#include <vector>
#include <memory>

#define CREATE_ENTITY(TYPE) Axon::createEntity<TYPE>()

namespace Axon
{
	typedef uint32_t ENTITY_ID;

	enum ControlMessage
	{
		GET_NEW_ID,
		CREATE,
		ONE_TO_ONE, 
		ONE_TO_MANY
	};

	class EntityBase;

	typedef std::vector<std::shared_ptr<EntityBase>> ENTITY_POOL;
	typedef std::map<ENTITY_ID, std::shared_ptr<EntityBase>> ENTITY_MAP;

	class EntityBase
	{
	private:
		inline static ENTITY_POOL g_inCreation;
		inline static ENTITY_MAP g_entities;
	public:
		static ENTITY_MAP ENTITIES() { return g_entities; }
		static ENTITY_POOL IN_CREATION() { return g_inCreation; }
	private:
		ENTITY_ID entityID;
		
		void dispatchCreationMessage();
		void update();
		
		void sendID();
	protected:
		EntityBase();
		~EntityBase();
	public:
		ENTITY_ID ID() const { return entityID; }
	
		virtual std::string toString() = 0;
		virtual void fromString(std::string) = 0;
	public:
		// Lifecycle

		virtual void onControlMessageRecived();
		virtual void onCreated();
		virtual void onInitDataRecived();
		virtual void onDestroyed();

		// Friends
		template <class T>
		friend std::weak_ptr<T> createEntity();
	};

	template<class T>
	std::weak_ptr<T> createEntity()
	{
		static_assert(std::is_base_of<EntityBase, T>());

		std::shared_ptr s_ptr = std::make_shared<T>();

		Axon::EntityBase::g_inCreation.push_back(s_ptr);

		s_ptr->dispatchCreationMessage();
		return std::weak_ptr<T>(s_ptr);
	}
}

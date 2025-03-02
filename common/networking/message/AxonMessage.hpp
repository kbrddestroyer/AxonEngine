/* AxonMessage.hpp */

#pragma once
#include <serialization/serialization.hpp>


namespace Networking
{
	enum FLAGS
	{
		F_VALIDATE		= 0b0001
	};

	/*
	*	AxonMessage is a low-level interface for data storage, serialization and network sharing
	*/
	class AxonMessage
	{
	private:
		size_t		size;
		void*		message;
		uint32_t	tag;


		void*		serialized;
		size_t		serializedSize;
	public:
		AxonMessage() = default;
		/**
		* Create new message from actual data (send mode)
		*/
		AxonMessage(void*, size_t, uint32_t = 0);
		/**
		* Create new message from serialized data (recv mode)
		*/
		AxonMessage(const char*, size_t);
		AxonMessage(const AxonMessage&);
		~AxonMessage();

		inline void* getSerialized() const { return serialized; }
		inline const char* getSerializedBuffer() const { return (const char*)serialized; }
		inline size_t getSerializedSize() const { return serializedSize; }
		inline void* getMessage() const { return message; }
		inline size_t getSize() const { return size; }
		inline uint32_t getTag() const { return tag; }
	}; 
}

// AxonMessage.hpp

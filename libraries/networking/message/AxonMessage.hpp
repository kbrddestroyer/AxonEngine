/* AxonMessage.hpp */

#pragma once
#include <serialization/serialization.hpp>

namespace Networking
{
	enum TAG_FLAGS
	{
		UNDEFINED	= 0,
		VALIDATE	= 1,
		ACKNOWLEDGE = 1 << 1,
		PARTIAL		= 1 << 2,
		FINISH		= 1 << 3
	};

	/*
	AxonMessage is a low-level interface for data storage, serialization and network sharing
	*/
	class AXON_DECLSPEC AxonMessage
	{
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

		void* getSerialized() const { return serialized; }
		const char* getSerializedBuffer() const { return static_cast<const char *>(serialized); }
		size_t getSerializedSize() const { return serializedSize; }
		void* getMessage() const { return message; }
		size_t getSize() const { return size; }
		TAG_T getTag() const { return tag; }
		uint16_t getHeader() const { return tag; }
	protected:
		TAG_T generateTag() const;
	private:
		size_t		size;
		void*		message;
		void*		serialized;
		size_t		serializedSize;
		TAG_T		tag;
	};
}

// AxonMessage.hpp

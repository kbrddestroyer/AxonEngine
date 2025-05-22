/* AxonMessage.hpp */

#pragma once
#include <serialization/serialization.hpp>

namespace Networking
{
    /*
     * MESSAGE TAG STRUCTURE:
     *
     * 8 bits of optional message data (such as partialID)
     * 8 bits of TAG_FLAGS
     *
     * FLAGS:
     * - UNDEFINED - default value
     * - VALIDATE - package should be validated and sender waits for validation package
     * - ACKNOWLEDGE - validation message for package with ID
     * - PARTIAL - message is being split into parts. Check data section for partID
     * - FINISH - last message in partial send
     */
	enum TAG_FLAGS
	{
		UNDEFINED	= 0,
		VALIDATE	= 1,
		ACKNOWLEDGE = 1 << 1,
		PARTIAL		= 1 << 2,
		FINISH		= 1 << 3
	};

    struct SerializedAxonMessage
    {
        size64_t size = 0;
        const char* bitstream = nullptr;
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
		AxonMessage(void*, size_t, uint8_t = 0, uint8_t = 0);

		/**
		* Create new message from legacy serialized data (recv mode)
		 *
		 * @see Networking::SerializedAxonMessage
		*/
		AxonMessage(const char*, size64_t);

        /**
         * Create new message from serialized message structure (preferred)
         */
        AxonMessage(const SerializedAxonMessage&);

		AxonMessage(const AxonMessage&);
		~AxonMessage();

        SerializedAxonMessage getSerialized() const;
		void* getMessage() const { return message; }
		size64_t getSize() const { return size; }
        uint8_t getFlags() const { return flags; }
        uint8_t getPartID() const { return partID; }

        void setPartID(uint8_t partID) { this->partID = partID; }
        void setFlags(uint8_t flags) { this->flags = flags; }
        void addFlag(TAG_FLAGS flag) { this->flags |= flag; }
        void removeFlag(TAG_FLAGS flag) { this->flags ^= flag; }

        AxonMessage split(size64_t);
    protected:
        static TAG_T generateTag(uint8_t, uint8_t);
        inline void decompressTag(TAG_T);
	private:
		size64_t	size;
		void*		message;
        uint8_t     partID = 0;
        uint8_t     flags = TAG_FLAGS::UNDEFINED;
        uint64_t    uniqueID;
	};
}

// AxonMessage.hpp

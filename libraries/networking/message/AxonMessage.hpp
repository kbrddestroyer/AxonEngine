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

    class AxonMessage;

    /*
     * Class, optimized for queued storage, bit split
     */
    class AXON_DECLSPEC SerializedAxonMessage
    {
    public:
        SerializedAxonMessage() = delete;
        SerializedAxonMessage(const char*, size_t);
        explicit SerializedAxonMessage(const AxonMessage&);

        explicit SerializedAxonMessage(const SerializedAxonMessage&);
        explicit SerializedAxonMessage(SerializedAxonMessage&, size_t, uintptr_t);
        explicit SerializedAxonMessage(SerializedAxonMessage&&) noexcept;

        ~SerializedAxonMessage();

        GETTER size_t getSize()  const { return size; }
        GETTER const char* getBits() const { return bitstream; }

        SerializedAxonMessage& operator=(const SerializedAxonMessage&);
        SerializedAxonMessage& operator=(SerializedAxonMessage&&) noexcept;
    protected:
        static TAG_T generateTag(uint8_t, uint8_t);
    private:
        size_t size = 0;
        uintptr_t offset = 0;
        const char* bitstream = nullptr;
        bool owning = true;

        friend class AxonMessage;
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
         * Create new message from serialized message structure (preferred)
         */
        explicit AxonMessage(const SerializedAxonMessage&);

		AxonMessage(const AxonMessage&);
		~AxonMessage();

        GETTER SerializedAxonMessage getSerialized() const;
        GETTER void* getMessage() const { return message; }
        GETTER size_t getSize() const { return size; }
        GETTER uint8_t getFlags() const { return flags; }
        GETTER uint8_t getPartID() const { return partID; }
        GETTER bool hasFlag(TAG_FLAGS flag) const { return flags & flag; }

        void setPartID(uint8_t id) { this->partID = id; }
        void setFlags(uint8_t flagSet) { this->flags = flagSet; }
        void addFlag(TAG_FLAGS flag) { this->flags |= flag; }
        void removeFlag(TAG_FLAGS flag) { this->flags ^= flag; }
    protected:
        inline void decompressTag(TAG_T);

        static uint64_t getUniqueID();
	private:
		size_t		size{};
		void*		message{};
        uint8_t     partID = 0;
        uint8_t     flags = TAG_FLAGS::UNDEFINED;
        uint64_t    uniqueID{};
	};
}

// AxonMessage.hpp

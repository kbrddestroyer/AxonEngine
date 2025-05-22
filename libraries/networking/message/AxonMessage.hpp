/* AxonMessage.hpp */

#pragma once
#include <serialization/serialization.hpp>
#include "netconfig.h"

#include <memory>
#include <bitset>

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
     */
	enum TAG_FLAGS
	{
		UNDEFINED	= 0,        /// default
		VALIDATE	= 1,        /// "how copy?"
		ACKNOWLEDGE = 1 << 1,   /// response package, "copy that!"
		PARTIAL		= 1 << 2,   /// set if message bits are part of a partial delivery
	};

    static_assert(sizeof(TAG_FLAGS) < sizeof(uint8_t));

    class AxonMessage;

    /*
     * Class, optimized for queued storage, bit split
     */
    class AXON_DECLSPEC SerializedAxonMessage
    {
    public:
        SerializedAxonMessage() = delete;
        SerializedAxonMessage(const char*, size_t);
        SerializedAxonMessage(const SerializedAxonMessage&);
        explicit SerializedAxonMessage(const AxonMessage&);
        explicit SerializedAxonMessage(SerializedAxonMessage&, size_t, uintptr_t, uint8_t, uint8_t, uint64_t);
        SerializedAxonMessage(SerializedAxonMessage&&) noexcept;

        ~SerializedAxonMessage();

        GETTER size_t getSize()  const { return size; }
        GETTER const char* getBits() const { return bytes; }
        GETTER uint8_t getFlags() const { return flags; }
        GETTER uint8_t getPartID() const { return partID; }

        SerializedAxonMessage& operator=(const SerializedAxonMessage&);
        SerializedAxonMessage& operator=(SerializedAxonMessage&&) noexcept;

        GETTER bool hasFlag(const TAG_FLAGS flag) const { return flags & flag; }

        // Mostly for debugging
        GETTER std::bitset<8> getFlagSet() const noexcept { return { flags }; }

        void setPartID(const uint8_t id) { this->partID = id; }

        // Resets old flags, sets new flag set
        void setFlags(const uint8_t flagSet) { this->flags = flagSet; }
        void addFlag(const TAG_FLAGS flag) { this->flags |= flag; }
        void removeFlag(const TAG_FLAGS flag) { this->flags ^= flag; }

		std::unique_ptr<SerializedAxonMessage> split();
    protected:
        static TAG_T generateTag(uint8_t, uint8_t);
        static uint64_t generateUniqueID() {
            static uint64_t uniqueID = 0;
            return uniqueID++;
        }
    private:
        size_t      size        = 0;
        uint64_t    uniqueID    = 0;
        uintptr_t   offset      = 0;
        bool        owning      = true;

        const char* bytes       = nullptr;

        uint8_t partID          = 0;
        uint8_t flags           = UNDEFINED;
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
		AxonMessage(const void*, size_t, uint8_t = 0, uint8_t = 0);

        /**
         * Create new message from serialized message structure (preferred)
         */
        explicit AxonMessage(const SerializedAxonMessage&);

		AxonMessage(const AxonMessage&);
		~AxonMessage();

        WGETTER(SerializedAxonMessage getSerialized());
        WGETTER(void* getMessage()) { return message; }
        WGETTER(size_t getSize()) { return size; }
    protected:
        inline void decompressTag(TAG_T);

        static uint64_t getUniqueID();
	private:
		size_t		size{};
		void*		message{};
        uint8_t     partID = 0;
        uint8_t     flags = UNDEFINED;
        uint64_t    uniqueID{};
	};
}

// AxonMessage.hpp

/* AxonMessage.hpp */

// TODO: Code styling, remove all shit pls

#pragma once
#include "serialization/serialization.hpp"

#include <memory>
#include <bitset>

namespace Networking
{
    /**
     * <b>MESSAGE TAG STRUCTURE:</b>
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
		UNDEFINED	= 0,        ///< default
		VALIDATE	= 1,        ///< "how copy?"
		ACKNOWLEDGE = 1 << 1,   ///< response package, "copy that!"
		PARTIAL		= 1 << 2,   ///< set if message bits are part of a partial delivery
		NETOBJ_INI	= 1 << 3,
		NETOBJ_REPL = 1 << 4,
		TO_ALL		= 1 << 5,
		SERV_STORE	= 1 << 6,

		ALL = UNDEFINED | VALIDATE | ACKNOWLEDGE | PARTIAL | NETOBJ_INI | NETOBJ_REPL | TO_ALL | SERV_STORE
	};

	static_assert(ALL == 0b1111111, "Flag set is incorrect. Check ALL value or manually edit this assert");
    class AxonMessage;

    /**
     * Class, optimized for queued storage, bit split
     */
    class AXON_DECLSPEC SerializedAxonMessage {
    public:
    	SerializedAxonMessage() = delete;
    	SerializedAxonMessage(const char*, size64_t);
    	SerializedAxonMessage(const SerializedAxonMessage&);
    	explicit SerializedAxonMessage(const AxonMessage&);
    	SerializedAxonMessage(SerializedAxonMessage&&) noexcept;

    	~SerializedAxonMessage();

    	WGETTER( size_t getSize() ) { return size; }
    	WGETTER ( const char* getBits() ) { return bytes; }

    	SerializedAxonMessage& operator=(const SerializedAxonMessage&);
    	SerializedAxonMessage& operator=(SerializedAxonMessage&&) noexcept;
    protected:
    	static TAG_T compressTag(uint8_t, uint8_t, uint16_t);

        void copy(const SerializedAxonMessage&);
        void move(SerializedAxonMessage&) noexcept;
    private:
    	size64_t    size        = 0;
    	bool        owning      = true;     // todo: `uint16_t _refcnt;` or shared_ptr around bytes array
    	const char* bytes       = nullptr;

        friend class AxonMessage;
    };

	/**
	* AxonMessage is a low-level interface for data storage, serialization and network sharing.
	*
	* Can be created in several ways. See constructors
	*/
	class AXON_DECLSPEC AxonMessage
	{
        struct MessageHolder {
            char * messagePtr = nullptr;
            size_t references = 0;
        };
	public:
        typedef std::unique_ptr<AxonMessage> UniqueAxonMessagePtr;
		AxonMessage() = default;

		/**
		* Create new message from actual data (send mode)
		* @param message data pointer
		* @param size message size, bytes
		* @param partID optional parameter
		* @param flags optional parameter, flag set
		*/
		AxonMessage(const void* message, size64_t size, uint8_t partID=0, uint8_t flags=0);

        /**
         * Create new message from serialized.
         * @param serialized SerializedAxonMessage object, containing compressed data
         */
        explicit AxonMessage(const SerializedAxonMessage &serialized);

        /**
         * Create acknowledge message with empty buffer
         * @param message message to create acknowledge message from (will copy uniqueID)
         * @param additionalFlags flags to add
         */
        AxonMessage(const AxonMessage &message, uint8_t additionalFlags);

        /**
         * Copy constructor
         */
        AxonMessage(const AxonMessage &);
		AxonMessage(const AxonMessage&, size64_t, uint8_t, uint8_t, uint64_t, size64_t);
		~AxonMessage();

        WGETTER(SerializedAxonMessage getSerialized());
        WGETTER(void* getMessage())
            { return (message && message->messagePtr) ? message->messagePtr + offset : nullptr; }
        WGETTER(size64_t getSize()) { return size; }
        WGETTER(uint16_t ID()) { return uniqueID; }
        WGETTER(uint8_t getFlags()) { return flags; }
        WGETTER(uint8_t getPartID()) { return partID; }
        WGETTER(bool hasFlag(const TAG_FLAGS flag)) { return flags & flag; }
        WGETTER(std::bitset<8> getFlagSet()) { return flags; }

        void setPartID(const uint8_t id) { this->partID = id; }
        void setFlags(const uint8_t flagSet) { this->flags = flagSet; }
        void addFlag(const TAG_FLAGS flag) { this->flags |= flag; }
        void removeFlag(const TAG_FLAGS flag) { this->flags ^= flag; }

        UniqueAxonMessagePtr split(size64_t);
        void append(const AxonMessage&);
    protected:
        static uint16_t generateUniqueID() {
            static uint16_t uniqueID = 0;
            if (uniqueID == INT16_MAX - 1)
                uniqueID = 0;
            return uniqueID++;
        }
        static void decompressTag(TAG_T, uint8_t*, uint8_t*, uint16_t*);
	private:
        MessageHolder *	message = nullptr;

        size64_t	size = 0;
        bool        owning  = true;
        uint8_t     partID = 0;
        uint8_t     flags = UNDEFINED;
        size64_t    offset = 0;
        uint16_t    uniqueID = generateUniqueID();
	};
}

// AxonMessage.hpp

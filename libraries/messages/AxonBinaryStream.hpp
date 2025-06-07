#pragma once

#include "AxonUtility.h"

namespace Networking {
    class AXON_DECLSPEC AxonBinaryStreamBase {
    public:
        AxonBinaryStreamBase() = default;
        AxonBinaryStreamBase(const AxonBinaryStreamBase&);
        AxonBinaryStreamBase(AxonBinaryStreamBase&&) noexcept;
        ~AxonBinaryStreamBase();

        AxonBinaryStreamBase& operator=(const AxonBinaryStreamBase&);
        AxonBinaryStreamBase& operator=(AxonBinaryStreamBase&&) noexcept;

        /* API */

        void append(const char*, size_t);
        GETTER const char* data() const noexcept { return containerPtr; }
        GETTER size_t size() const noexcept { return containerSize; }
        void clear();
    protected:
        char* containerPtr = nullptr;
        size_t containerSize = 0;
    };

    class AXON_DECLSPEC AxonBinaryStream : public AxonBinaryStreamBase {
    public:
        AxonBinaryStream() = default;
    };
}

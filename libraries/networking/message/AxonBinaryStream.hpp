#pragma once

#include <AxonUtility.h>

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
        const char* data() const noexcept { return containerPtr; }
        size_t size() const noexcept { return containerSize; }
        void clear();
    private:
        char* containerPtr = nullptr;
        size_t containerSize = 0;
    };
}

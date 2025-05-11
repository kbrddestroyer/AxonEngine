#include "AxonBinaryStream.hpp"
#include <utility>
#include <memory.h>

namespace Networking {
    AxonBinaryStreamBase::AxonBinaryStreamBase(const AxonBinaryStreamBase & other) {
        if (!other.containerPtr)
            return;

        containerPtr = new char[other.containerSize];

        memcpy(containerPtr, other.containerPtr, other.containerSize);
        containerSize = other.containerSize;
    }

    AxonBinaryStreamBase::AxonBinaryStreamBase(AxonBinaryStreamBase && other) noexcept :
        containerPtr(other.containerPtr)
    {
        containerSize = other.containerSize;
        other.containerSize = 0;
        other.containerPtr = nullptr;
    }

    AxonBinaryStreamBase::~AxonBinaryStreamBase() {
        delete[] containerPtr;
        containerSize = 0;
    }

    AxonBinaryStreamBase& AxonBinaryStreamBase::operator=(const AxonBinaryStreamBase & other) {
        if (this == &other) return *this;

        if (!other.containerPtr)
        {
            if (containerPtr)
            {
                delete[] containerPtr;
                containerSize = 0;
            }
            return *this;
        }

        containerPtr = new char[other.containerSize];
        memcpy(containerPtr, other.containerPtr, other.containerSize);
        containerSize = other.containerSize;
        return *this;
    }

    AxonBinaryStreamBase& AxonBinaryStreamBase::operator=(AxonBinaryStreamBase && other) noexcept {
        if (this == &other) return *this;

        if (!other.containerPtr)
        {
            if (containerPtr)
            {
                delete[] containerPtr;
                containerSize = 0;
            }
            return *this;
        }

        containerPtr = new char[other.containerSize];
        memcpy(containerPtr, other.containerPtr, other.containerSize);
        containerSize = other.containerSize;
        return *this;
    }

    void Networking::AxonBinaryStreamBase::append(const char* data, std::size_t dataSize)
    {
        char* containerBufferPointer = new char[containerSize + dataSize];
        memcpy(containerBufferPointer, containerPtr, containerSize);
        memcpy(containerBufferPointer + containerSize, data, dataSize);
        delete[] containerPtr;
        containerPtr = containerBufferPointer;
        containerSize += dataSize;
    }

    void Networking::AxonBinaryStreamBase::clear() {
        delete[] containerPtr;
        containerSize = 0;
    }
}

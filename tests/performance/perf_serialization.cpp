#include <MemoryGuard/MemoryGuard.hpp>

#include <chrono>
#include <iostream>

#include <iomanip>
#include <bitset>
#include <serialization/serialization.hpp>

int main () {
    ENABLE_MEMGUARD
    constexpr char buffer[] = "Hello World! Some big data here is awaiting serialization";

    const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    size64_t total;
    const char* result = serialize(buffer, sizeof(buffer), 2, &total);

    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    DISABLE_MEMGUARD
    std::cout <<
            std::left << std::setw(50) << "Execution time: " <<
                std::chrono::duration_cast<std::chrono::nanoseconds> ( end - start ).count() << "ns" << std::endl <<
                    std::setw(50) << "Serialization allocated " << total << " bytes of memory"
    << std::endl;

    for (size64_t i = 0; i < total; i++) {
        std::cout << std::setw(15) << std::bitset<8>( result[i] ) << std::setw(5) << result[i] << std::setw(5) << static_cast<uint16_t>(result[i]) << std::endl;
    }
    ENABLE_MEMGUARD
    free((void*) result);
    return 0;
}

MEMGUARD_REPORT
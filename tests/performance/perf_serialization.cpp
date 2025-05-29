#include "perftest.hpp"

#include <chrono>
#include <iostream>

#include <iomanip>
#include <bitset>
#include <serialization/serialization.hpp>


#define ENABLE_GUARD MemoryGuard::Singleton()->enableGuard()
#define DISABLE_GUARD MemoryGuard::Singleton()->disableGuard()

int main () {
    ENABLE_GUARD;

    constexpr char buffer[] = "Hello World! Some big data here is awaiting serialization";

    const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    size64_t total;
    const char* result = serialize(buffer, sizeof(buffer), 2, &total);

    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    DISABLE_GUARD;
    std::cout <<
            std::left << std::setw(50) << "Execution time: " <<
                std::chrono::duration_cast<std::chrono::nanoseconds> ( end - start ).count() << "ns" << std::endl <<
                    std::setw(50) << "Serialization allocated " << total << " bytes of memory"
    << std::endl;

    for (size64_t i = 0; i < total; i++) {
        std::cout << std::setw(15) << std::bitset<8>( result[i] ) << std::setw(5) << result[i] << std::setw(5) << static_cast<uint16_t>(result[i]) << std::endl;
    }
    ENABLE_GUARD;
    return 0;
}

__attribute__((destructor))
void destroy() {
    if (!MemoryGuard::Singleton()->empty()) {
        std::cout << std::endl;
        std::cerr << "MemoryGuard detected memory leak!" << std::endl;
        MemoryGuard::Singleton()->disableGuard();
        std::cerr << "Leaked " << MemoryGuard::Singleton()->allocated() << " bytes of memory :c" << std::endl;
        std::cerr << "For more information, add breakpoint into guard hook functions and see, what causes memory leak"
                    << std::endl;
     }
}
#pragma once
#if defined(__unix) || defined(__unix__)
#include <cstdio>
#include <cstdint>
#include <iostream>

extern "C" void add_to_guard(void*, size_t);
extern "C" void remove_from_guard(void*);

namespace Testing {
    struct MemoryNode {
        MemoryNode *next;

        void    *ptr;
        size_t  size;
    };

    class MemoryGuard {
    public:
        MemoryGuard();
        MemoryGuard(const MemoryGuard&) = delete;
        MemoryGuard& operator=(const MemoryGuard&) = delete;
        ~MemoryGuard();

        void insert(void*, size_t);
        void pop(void*);
        [[nodiscard]] size_t total() const;
        void setEnabled(bool enabled) { this->isEnabled = enabled; }
        static MemoryGuard* instance();

       [[nodiscard]] MemoryNode *node() const noexcept { return head; }
    private:
        MemoryNode *head = nullptr;
        bool isEnabled = false;
    };
} // Testing

#define ENABLE_MEMGUARD Testing::MemoryGuard::instance()->setEnabled(true);
#define DISABLE_MEMGUARD Testing::MemoryGuard::instance()->setEnabled(false);

#define MEMGUARD_REPORT                                                                             \
__attribute__((destructor))                                                                         \
void memguard_report() {                                                                            \
    DISABLE_MEMGUARD                                                                                \
    std::cerr.flush();                                                                              \
    std::cout.flush();                                                                              \
    Testing::MemoryGuard *g_guard = Testing::MemoryGuard::instance();                               \
    Testing::MemoryNode *head = g_guard->node();                                                    \
    if (head) {                                                                                     \
        while (head) {                                                                              \
            std::cerr << "0x" << reinterpret_cast<uintptr_t>(head->ptr) << " " <<                   \
            head->size << std::endl;                                                                \
            head = head->next;                                                                      \
        }                                                                                           \
        std::cerr << "Total leaked memory: " << g_guard->total() << std::endl;                      \
    }                                                                                               \
}
#else
#pragma message( __FILE__ " MemoryGuard is not supported on current platform" )
#pragma message( "MemoryGuard is disabled" )

#define ENABLE_MEMGUARD
#define DISABLE_MEMGUARD
#define MEMGUARD_REPORT
#endif

#include "MemoryGuard.hpp"

#if defined(__unix) || defined(__unix__)
extern "C" void init_hooks();
extern void* (*real_malloc) (size_t);
extern void* (*real_calloc) (size_t, size_t);
extern void* (*real_realloc) (void*, size_t);
extern void  (*real_free) (void*);

void add_to_guard(void *ptr, size_t size) {
    Testing::MemoryGuard::instance()->insert(ptr, size);
}

void remove_from_guard(void *ptr) {
    Testing::MemoryGuard::instance()->pop(ptr);
}

namespace Testing {
    MemoryGuard::MemoryGuard() {
        init_hooks();
    }

    MemoryGuard::~MemoryGuard() {
        while (head) {
            MemoryNode *dummy = head;
            head = head->next;
            real_free(dummy);
        }
    }

    void MemoryGuard::insert(void *ptr, size_t size) {
        if (!isEnabled) return;

        MemoryNode *node = reinterpret_cast<MemoryNode*>(real_malloc(sizeof(MemoryNode)));
        node->ptr = ptr;
        node->size = size;
        node->next = head;
        head = node;
    }

    void MemoryGuard::pop(void *ptr) {
        if (!head) return;
        if (head->ptr == ptr) {
            // Remove if already on head
            MemoryNode* dummy = head;
            head = head->next;
            real_free(dummy);
            return;
        }

        MemoryNode *node = head;
        while (node->next && node->next->ptr != ptr) node = node->next;

        if (!node->next) return;  // Not found

        // Remove
        MemoryNode* dummy = node->next;
        node->next = node->next->next;
        real_free(dummy);
    }

    size_t MemoryGuard::total() const {
        size_t total = 0;
        MemoryNode *node = head;
        while (node) {
            total += node->size;
            node = node->next;
        }

        return total;
    }

    MemoryGuard *MemoryGuard::instance() {
        static MemoryGuard g_guard;
        return &g_guard;
    }
} // Testing
#endif

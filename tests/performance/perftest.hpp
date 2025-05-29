#pragma once
#if defined(__unix__)
#include <malloc.h>
#include <dlfcn.h>


extern "C" void* (*real_malloc) (size_t) = nullptr;
extern "C" void* (*real_calloc) (size_t, size_t) = nullptr;
extern "C" void* (*real_realloc) (void*, size_t) = nullptr;
extern "C" void (*real_free) (void*) = nullptr;

__attribute__((constructor))
static void init_hooks() {
    real_malloc = (void* (*) (size_t)) dlsym(RTLD_NEXT, "malloc");
    real_calloc = (void* (*) (size_t, size_t)) dlsym(RTLD_NEXT, "calloc");
    real_realloc = (void* (*) (void*, size_t)) dlsym(RTLD_NEXT, "realloc");
    real_free = (void (*) (void*)) dlsym(RTLD_NEXT, "free");
}

struct GuardNode {
    void* ptr               = nullptr;
    size_t size_allocated   = 0;
    GuardNode* next         = nullptr;
};

class MemoryGuard {
public:
    MemoryGuard() {
        if (! real_malloc || ! real_free)
            init_hooks();
    }

    ~MemoryGuard() {
        while (head) {
            GuardNode* dummy = head;
            head = head->next;
            real_free(dummy->ptr);
            real_free(dummy);
        }
    }

    void enableGuard() { isEnabled = true; }
    void disableGuard() { isEnabled = false; }

    void insert(void* ptr, size_t size) {
        if (!isEnabled) return;

        GuardNode* node = (GuardNode*) real_malloc(sizeof(GuardNode));
        node->next = head;
        node->ptr = ptr;
        node->size_allocated = size;

        head = node;
    }

    void pop(void* ptr) {
        GuardNode* node = head;

        if (!node) return;
        if (node->ptr == ptr) {
            head = node->next;
            real_free(node);
            return;
        }
        while (node->next && node->next->ptr != ptr) node = node->next;
        if (!node->next) return;

        GuardNode* dummy = node->next;
        node->next = node->next->next;
        real_free(dummy);
    }

    bool empty() { return head == nullptr; }

    static MemoryGuard* Singleton() {
        static MemoryGuard g_guard;
        return &g_guard;
    }

    [[nodiscard]] size_t allocated() const noexcept {
        GuardNode *node = head;
        size_t acc = 0;
        while (node) {
            acc += node->size_allocated;
            node = node->next;
        }
        return acc;
    }

private:
    GuardNode* head = nullptr;
    bool isEnabled = false;
};

extern "C" void* malloc(size_t size) noexcept {
    if (!real_malloc)
        init_hooks();
    void *ptr = real_malloc(size);
    MemoryGuard::Singleton()->insert(ptr, size);
    return ptr;
}

extern "C" void *calloc(size_t nmem, size_t size) noexcept {
    if (!real_calloc)
        init_hooks();

    void *ptr = real_calloc(nmem, size);
    MemoryGuard::Singleton()->insert(ptr, nmem * size);
    return ptr;
}

extern "C" void *realloc(void *ptr, size_t size) noexcept {
    if (!real_realloc)
        init_hooks();

    ptr = real_realloc(ptr, size);
    MemoryGuard::Singleton()->insert(ptr, size);
    return ptr;
}

extern "C" void free(void *ptr) noexcept {
    if (!real_free)
        init_hooks();
    MemoryGuard::Singleton()->pop(ptr);
    real_free(ptr);
}

#endif

#include "memutils.h"

#if defined(__unix) || defined(__unix__)
extern void add_to_guard(void*, size_t);
extern void remove_from_guard(void*);

void init_hooks() {
    if (!real_malloc)
        real_malloc     = dlsym(RTLD_NEXT, "malloc");
    if (!real_calloc)
        real_calloc     = dlsym(RTLD_NEXT, "calloc");
    if (!real_realloc)
        real_realloc    = dlsym(RTLD_NEXT, "realloc");
    if (!real_free)
        real_free       = dlsym(RTLD_NEXT, "free");
}

void* malloc(size_t size) {
    if (!real_malloc) init_hooks();
    void *ptr = real_malloc(size);
    add_to_guard(ptr, size);
    return ptr;
}

void* calloc(size_t nmem, size_t size) {
    if (!real_calloc) init_hooks();
    void *ptr = real_calloc(nmem, size);
    add_to_guard(ptr, nmem * size);
    return ptr;
}

void* realloc(void *ptr, size_t size) {
    if (!real_realloc) init_hooks();
    ptr = real_realloc(ptr, size);
    add_to_guard(ptr, size);
    return ptr;
}

void free(void *ptr) {
    if (!real_free) init_hooks();
    remove_from_guard(ptr);
    real_free(ptr);
}
#endif

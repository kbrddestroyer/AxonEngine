#ifndef AXON_MEMUTILS_H
#define AXON_MEMUTILS_H
#if defined(__unix) || defined(__unix__)
#include <dlfcn.h>
#include <malloc.h>

// Hooks to "real" functions

void* (*real_malloc) (size_t) = NULL;
void* (*real_calloc) (size_t, size_t) = NULL;
void* (*real_realloc) (void*, size_t) = NULL;
void (*real_free) (void*) = NULL;

void *malloc(size_t size);
void *calloc(size_t nmem, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
#endif
#endif
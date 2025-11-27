#include "pallocator.h"

void* palloc_alloc(pallocator_t *alloc, size_t size, size_t len) {
    return alloc->alloc(alloc->data, size, len);
}
void palloc_free(pallocator_t *alloc, void *ptr) {
    alloc->free(alloc->data, ptr);
}

void palloc_cleanup(pallocator_t *alloc) {
    alloc->cleanup(alloc->data);
}

void palloc_sanity_check(pallocator_t *alloc, void *ptr) {
    alloc->sanity_check(alloc->data, ptr);
}


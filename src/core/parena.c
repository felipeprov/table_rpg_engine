#include "pallocator.h"

#include <stddef.h>
#include <stdint.h>

typedef struct pallocator_t {
  void *data;
  void (*init)(void *data, size_t prealloc_size);
  void *(*alloc)(void *data, size_t size, size_t len);
  void (*free)(void *data, void *ptr);
  void (*cleanup)(void *data);
  void (*sanity_check)(void *data, void *ptr);
} pallocator_t;

static void init(void *data, size_t alloc) {}
static void *alloc(void *data, size_t size, size_t len) {}
static void free(void *data, void *ptr) {}
static void cleanup(void *data) {}
static void sanity_check(void *data, void *ptr) {}

pallocator_t arena_allocator = {
    .init = init,
    .cleanup = cleanup,
    .free = free,
    .alloc = alloc,
    .sanity_check = sanity_check,
};

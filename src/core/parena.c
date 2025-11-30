#include "pallocator.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct arena_data_t {
    uint8_t *memory;
    size_t size;
    size_t offset;
} arena_data_t;

static void* arena_init(size_t alloc) {
  size_t total_size = sizeof(arena_data_t) + alloc;
  arena_data_t *data = (arena_data_t *)malloc(total_size);
  data->memory = (uint8_t *)(data + 1);
  data->size = alloc;
  data->offset = 0;
  return data;
}

static void *arena_alloc(void *data, size_t size, size_t len) {
  // align size to 8 bytes
  size_t aligned_size = (size + 7) & ~7;

  arena_data_t *arena = (arena_data_t *)data;
  size_t total_size = aligned_size * len;
  if (arena->offset + total_size > arena->size) {
      return NULL; // Out of memory
  }
  void *ptr = arena->memory + arena->offset;
  arena->offset += total_size;
  
  return ptr;
}

static void arena_free(void *data, void *ptr) {
    // No-op for arena allocator
}

static void arena_cleanup(void *data) {
    free(data);
}

static void arena_sanity_check(void *data, void *ptr) {
    // No-op for arena allocator
}

static void arena_frame_free(void* data, void *ptr) {
    arena_data_t *arena = (arena_data_t *)data;
    arena->offset = 0; // Reset offset to free all allocations
}

pallocator_t arena_allocator = {
    .init = arena_init,
    .cleanup = arena_cleanup,
    .free = arena_free,
    .alloc = arena_alloc,
    .sanity_check = arena_sanity_check,
    .frame_free = arena_frame_free,
};


pallocator_t *palloc_arena_create(size_t size) {
    size_t total = sizeof(pallocator_t) + size + sizeof(arena_data_t);
    pallocator_t *alloc = (pallocator_t *)malloc(total);
    *alloc = arena_allocator;
    alloc->data = alloc->init(size);
    return alloc;
}
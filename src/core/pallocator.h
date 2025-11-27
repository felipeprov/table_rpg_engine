#pragma once

#include <stddef.h>
#include <stdint.h>

// Simple allocator,
// In this project this is the only
// authorized way to allocate memory
typedef struct pallocator_t {
  void *data;
  void* (*init)(size_t prealloc_size);
  void *(*alloc)(void *data, size_t size, size_t len);
  void (*free)(void *data, void *ptr);
  void (*cleanup)(void *data);
  void (*sanity_check)(void *data, void *ptr);
  void (*frame_free)(void* data, void *ptr);
} pallocator_t;


pallocator_t *palloc_arena_create(size_t size);
void* palloc_alloc(pallocator_t *alloc, size_t size, size_t len);
void palloc_free(pallocator_t *alloc, void *ptr);
void palloc_cleanup(pallocator_t *alloc);
void palloc_sanity_check(pallocator_t *alloc, void *ptr);

// Arena allocator convenience functions
void palloc_frame_free(pallocator_t *alloc);
#pragma once
#include <stddef.h>
#include <stdint.h>

#include "pallocator.h"

// Main array type,
// users are not supposed to manipulate it directly
// but it allows to keep the control of memory
typedef struct parray_t {
  size_t size;
  size_t len;
  uint8_t *ptr;
  uint8_t data[];
} parray_t;

void *parray_create(pallocator_t *alloc, size_t len, size_t size);
void parray_delete(pallocator_t *alloc, void *ptr);

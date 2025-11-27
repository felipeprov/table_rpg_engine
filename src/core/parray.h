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
void *parray_get(parray_t *array, size_t index);
size_t parray_len(parray_t *array);
size_t parray_size(parray_t *array);

void* parray_push(parray_t* array, void *item);
void* parray_pop(parray_t* array);


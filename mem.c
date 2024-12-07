#include <stdlib.h>
#include "mem.h"

Psc_Memory_pool init_pool() {
  Psc_Memory_pool p;
  p.mem = malloc(sizeof(void) * 1024);
  p.offset = 0;
  p.size = 1024;
  return p;
}

void *psc_alloc(Psc_Memory_pool *p, size_t size) {
  if (p->offset + size <= p->size) {
    void*ptr = p->mem + p->offset;
    p->offset += size;
    return ptr;
  } else {
    return NULL;
  }
}

Psc_Memory_pool psc_realloc(Psc_Memory_pool *p, size_t size) {
  p->mem = realloc(p->mem, p->size + size);
  p->size += size;
  return *p;
}

void psc_free_pool(Psc_Memory_pool *p) {
  free(p->mem);
  p->mem = NULL;
  p->offset = 0;
} 

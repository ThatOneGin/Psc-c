#ifndef PSC_MEM
#define PSC_MEM
#include <stdlib.h>
#define psc_pool_size 1024

typedef struct {
  void *mem;
  size_t offset;
  size_t size;
}Psc_Memory_pool;

Psc_Memory_pool init_pool();
void *psc_alloc(Psc_Memory_pool *p, size_t size);
void psc_free_pool(Psc_Memory_pool *p);
Psc_Memory_pool psc_realloc(Psc_Memory_pool *p, size_t size);
#endif
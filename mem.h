#ifndef PSC_MEM
#define PSC_MEM
#include <stdlib.h>
#define psc_pool_size 1024

typedef struct {
  void *mem;
  size_t offset;
}Psc_Memory_pool;

Psc_Memory_pool init_pool();
void *psc_alloc(Psc_Memory_pool *p, size_t size);
void psc_free_pool(Psc_Memory_pool *p);
#endif
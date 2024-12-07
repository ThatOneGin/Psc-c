#include "../mem.c"
#include "../mem.h"

int main() {
  Psc_Memory_pool p = init_pool();
  p = psc_realloc(&p, 120);

  void *a = psc_alloc(&p, 1024);

  psc_free_pool(&p);
  return 0;
}
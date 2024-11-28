#include "state.h"
#include "mem.h"
#include <stdio.h>

/*
[=Variable management functions=]
*/

int set_variable(Psc_State *P, char *name, int type, Psc_value value) {
  if (P->G_ENV.size > P->G_ENV.capacity) {
    return -1;
  }

  P->G_ENV.variables[P->G_ENV.size] = (psc_variable) {
    .name = name,
    .type = type,
    .value = value
  };
  P->G_ENV.size += 1;

  return 0;
}
psc_variable get_variable(Psc_State *P, char *name) {
  for (size_t i = 0; i < P->G_ENV.size; i++) {
    if (P->G_ENV.variables[i].name == name) {
      return P->G_ENV.variables[i];
    }
  }

  printf("Couldn't find variable named %s", name);
  exit(1);
}

int reset_variable(Psc_State *P, char *name, Psc_value value) {
  for (size_t i = 0; i < P->G_ENV.size; i++) {
    if (P->G_ENV.variables[i].name == name) {
      P->G_ENV.variables[i].value = value;
      return 0;
    }
  }

  return -1;
}

Psc_State init_psc_state() {
  Psc_State p = {0};
  p.stack.sz = 0;
  p.stack.cap = 1024;

  return p;
}

void close_psc_state(Psc_State *P) {
  free(P);
  P = NULL;
}
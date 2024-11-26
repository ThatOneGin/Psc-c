#include "state.h"
#include <assert.h>

void psc_push_int(Psc_State *P, int v) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_Integer,
    .value.p_int = v
  };
  P->stack.sz += 1;
}
void psc_push_num(Psc_State *P, double v) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_Number,
    .value.p_num = v
  };
  P->stack.sz += 1;
}
void psc_push_str(Psc_State *P, char *str) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_String,
    .value.p_str = str
  };
  P->stack.sz += 1;
}
int psc_is_int(Psc_State *P, int idx) {
  assert((size_t)idx < P->stack.sz);
  return P->stack.stack[idx].kind == Kind_Integer;
}

int psc_is_num(Psc_State *P, int idx) {
  assert((size_t)idx < P->stack.sz);
  return P->stack.stack[idx].kind == Kind_Number;
}

int psc_is_str(Psc_State *P, int idx) {
  assert((size_t)idx < P->stack.sz);
  return P->stack.stack[idx].kind == Kind_String;
}
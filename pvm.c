#include <stdio.h>
#include "parser.h"
#include "pvm.h"
#include "state.h"
#include <assert.h>

void psc_push_int(Psc_State *P, int v) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_Integer,
    .value = (void *)(&v)
  };
  P->stack.sz += 1;
}
void psc_push_num(Psc_State *P, double v) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_Number,
    .value = (void *)(&v)
  };
  P->stack.sz += 1;
}
void psc_push_str(Psc_State *P, char *str) {
  assert(P->stack.sz < P->stack.cap);
  P->stack.stack[P->stack.sz] = (Psc_value) {
    .kind = Kind_String,
    .value = (void *)str
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

Psc_value int_to_value(int a) {
  return (Psc_value){.value = (void *)&a, .kind = Kind_Integer};
}

Psc_value evaluate_binary_operation(AstExpr *a, Psc_State *P) {
  int lhs = *(int *)evaluate(a->lhs, P).value;
  int rhs = *(int *)evaluate(a->rhs, P).value;

  int result;
  switch (a->op[0]) {
    case '+':
      result = lhs + rhs;
      return int_to_value(lhs + rhs);
    case '-':
      result = lhs - rhs;
      return int_to_value(lhs - rhs);
    case '/':
      result = lhs / rhs;
      return int_to_value(lhs / rhs);
    case '*':
      result = lhs * rhs;
      return int_to_value(lhs * rhs);
    default:
      return int_to_value(0);
  }

  psc_push_int(P, result);
}

Psc_value evaluate(Ast_T *a, Psc_State *P) {
  switch (a->kind) {
  case AstInt:
    return int_to_value(((AstNumber *)a)->value);
  case Expr:
    return evaluate_binary_operation(((AstExpr *)a), P);
  default:
    printf("Case not supported %d", a->kind);
    exit(1);
  }
}
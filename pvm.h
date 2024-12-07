#ifndef PSC_PVM_HEADER
#define PSC_PVM_HEADER
#include "state.h"
#include "parser.h"

void psc_push_int(Psc_State *P, int v);
void psc_push_num(Psc_State *P, double v);
void psc_push_str(Psc_State *P, char *str);
int  psc_is_int(Psc_State *P, int idx);
int  psc_is_num(Psc_State *P, int idx);
int  psc_is_str(Psc_State *P, int idx);
Psc_value int_to_value(int a);
Psc_value evaluate_binary_operation(AstExpr *a, Psc_State *P);
Psc_value evaluate(Ast_T *a, Psc_State *P);
#endif
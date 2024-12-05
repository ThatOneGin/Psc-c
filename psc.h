#ifndef PSC_H_
#define PSC_H_
#include "parser.h"
#include "state.h"

Psc_value int_to_value(int a);
Psc_value evaluate_binary_operation(AstExpr *a, Psc_State *P);
Psc_value evaluate(Ast_T *a, Psc_State *P);
#endif

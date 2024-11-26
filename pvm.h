#ifndef PSC_PVM_HEADER
#define PSC_PVM_HEADER
#include "state.h"

void psc_push_int(Psc_State *P, int v);
void psc_push_num(Psc_State *P, double v);
void psc_push_str(Psc_State *P, char *str);
int  psc_is_int(Psc_State *P, int idx);
int  psc_is_num(Psc_State *P, int idx);
int  psc_is_str(Psc_State *P, int idx);
#endif
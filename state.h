#ifndef Psc_state_header
#define Psc_state_header
#include "mem.h"
#include <stdlib.h>

typedef enum {
  Kind_Integer,
  Kind_Number,
  Kind_String
}value_kinds;

typedef int Psc_int;
typedef double Psc_number;
typedef char *Psc_str;

typedef struct {
  value_kinds kind;
  void *value;
}Psc_value;

typedef struct {
  char *name;
  Psc_value value;
  int type;
} psc_variable;

typedef struct {
  psc_variable *variables;
  size_t size;
  size_t capacity;
} Variable_Global_Env;

typedef struct {
  Psc_value stack[1024];
  size_t sz;
  size_t cap;
}Psc_Stack;

typedef struct {
  Variable_Global_Env G_ENV;
  Psc_Stack stack;
}Psc_State;

Psc_State init_psc_state();
int set_variable(Psc_State *P, char *name, int type, Psc_value value);
psc_variable get_variable(Psc_State *P, char *name);
int reset_variable(Psc_State *P, char *name, Psc_value value);
#endif
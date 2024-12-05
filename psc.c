#include "psc.h"
#include "dump_ast.c"
#include "lex.c"
#include "mem.h"
#include "parser.c"
#include "parser.h"
#include "pvm.c"
#include "state.c"
#include "state.h"
#include <stdio.h>

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

char *openfile(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    perror("Error opening file");
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  int f_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = (char *)malloc(f_size + 1);
  if (buffer == NULL) {
    perror("Memory allocation failed");
    fclose(f);
    exit(1);
  }

  fread(buffer, 1, f_size, f);

  buffer[f_size] = '\0';

  fclose(f);
  return buffer;
}

int main(int argc, char **argv) {
  if (argc < 1) {
    printf("Usage: ./psc <input file>\n");
    exit(1);
  }

  Psc_Memory_pool pool = init_pool();
  Psc_State P = init_psc_state();

  char *fl = openfile(argv[1]); // for now, only one file

  Lexer l = init_lexer(fl, &pool);
  tokenize(&l);

  Parser p = init_parser(l.tokens);
  AstList ast = make_ast(&p, &pool);

  freelist(l);
  free_ast_list(ast);
  free(fl);
  psc_free_pool(&pool);
  return 0;
}

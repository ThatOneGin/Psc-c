#include "mem.h"
#include "pvm.c"
#include "dump_ast.c"
#include "lex.c"
#include "parser.c"
#include "parser.h"
#include "state.c"
#include <stdio.h>

int evaluate(Ast_T *a, Psc_State *P) {
  switch (a->kind) {
    case Expr:
      int lhs = evaluate(((AstExpr *)a)->lhs, P);
      int rhs = evaluate(((AstExpr *)a)->rhs, P);
      int result;
      char *op = ((AstExpr *)a)->op;

      if (op[0] == '+') {
        psc_push_int(P, lhs + rhs);
        result = lhs + rhs;
      } else if (op[0] == '-') {
        psc_push_int(P, lhs - rhs);
        result = lhs - rhs;
      } else if (op[0] == '*') {
        psc_push_int(P, lhs * rhs);
        result = lhs * rhs;
      } else if (op[0] == '/') {
        psc_push_int(P, lhs / rhs);
        result = lhs / rhs;
      }
      return result;
    case AstInt:
      return ((AstNumber *)a)->value;
    default:
      printf("Case not supporetd. only handled cases are astint and expression.\n");
      exit(1);
  }

  return 0;
}

void evaluate_list(AstList a, Psc_State *P) {
  for (int i = 0; i < a.size+1; i++) {
    evaluate(a.data[i], P);
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
  
  Lexer l = init_lexer(fl);
  tokenize(&l);
  
  Parser p = init_parser(l.tokens);
  AstList ast = make_ast(&p);

  evaluate_list(ast, &P);
  
  freelist(l);
  free_ast_list(ast);
  free(fl);
  psc_free_pool(&pool);
  return 0;
}

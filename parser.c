#include "parser.h"
#include "lex.h"
#include "mem.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define initial_ast_cap 2

void free_ast(Ast_T *ast) {
  if (ast->kind == AstIdent) {
    free((AstIdentifier *)ast);
  } else if (ast->kind == AstInt) {
    free((AstNumber *)ast);
  } else if (ast->kind == Expr) {
    free_ast(((AstExpr *)ast)->lhs);
    free_ast(((AstExpr *)ast)->rhs);
    free((AstExpr *)ast);
  }
}

AstIdentifier create_astidentifier(char *value, Psc_Memory_pool p) {
  AstIdentifier a = {0}; //= (AstIdentifier *) malloc(sizeof(AstIdentifier));
  a.value = value;
  a.base.kind = AstIdent;

  return a;
}

AstNumber create_astint(char *value, Psc_Memory_pool p) {
  AstNumber a = {0}; //= (AstNumber *) malloc(sizeof(AstNumber));
  a.value = atoi(value);
  a.base.kind = AstInt;

  return a;
}

AstExpr create_astbinop(Ast_T *lhs, Ast_T *rhs, char *op, Psc_Memory_pool p) {
  AstExpr a = {0}; // = (AstExpr *) malloc(sizeof(AstExpr));
  a.base.kind = Expr;
  a.lhs = lhs;
  a.rhs = rhs;
  a.op = op;

  return a;
}

Token parser_eat(Parser *p) {
  // assert(p->index <= p->tokens.size);
  assert(p->tokens.data[p->index].type != PSC_EOF);
  Token tk = p->tokens.data[p->index];
  p->tokens.size -= 1;
  p->index += 1;
  return tk;
};

Token parser_at(Parser *p) { return p->tokens.data[p->index]; };

Ast_T *parse_primary_expr(Parser *p, Psc_Memory_pool pool) {
  Token tk = parser_eat(p);
  Ast_T *n;

  switch (tk.type) {
  case Identifier:
    AstIdentifier a_i = create_astidentifier(tk.value, pool);
    n = (Ast_T *)(&a_i);
    return n;

  case Number:
    AstNumber a_n = create_astint(tk.value, pool);
    n = (Ast_T *)(&a_n);
    return n;
  default:
    printf("Type not handled as primary expression. '%s'\n", tk.value);
    exit(1);
  }
};

Ast_T *parse_multiplicitave_expr(Parser *p, Psc_Memory_pool pool) {
  Ast_T *left = parse_primary_expr(p, pool);

  while (strcmp(parser_at(p).value, "/") == 0 ||
         strcmp(parser_at(p).value, "*") == 0) {
    char *operator= parser_eat(p).value;
    Ast_T *right = parse_primary_expr(p, pool);

    // return (Ast_T *) create_astbinop(left, right, operator);
    AstExpr a = create_astbinop(left, right, operator, pool);
    left = (Ast_T *)(&a);

    return left;
  }

  return left;
};

Ast_T *parse_additive_expr(Parser *p, Psc_Memory_pool pool) {
  Ast_T *left = parse_multiplicitave_expr(p, pool);

  while (strcmp(parser_at(p).value, "+") == 0 ||
         strcmp(parser_at(p).value, "-") == 0) {
    char *operator= parser_eat(p).value;
    Ast_T *right = parse_multiplicitave_expr(p, pool);

    AstExpr a = create_astbinop(left, right, operator, pool);
    left = (Ast_T *)(&a);

    return left;
  }

  return left;
};

Ast_T *parse_stmt(Parser *p, Psc_Memory_pool pool) {
  return parse_additive_expr(p, pool);
}

AstList init_astlist() {
  AstList a;
  a.data = malloc(sizeof(Ast_T *) * initial_ast_cap);
  a.size = 0;
  a.capacity = initial_ast_cap;
  return a;
}

void append_to_astlist(AstList list, Ast_T *a) {
  if (list.size == list.capacity) {
    list.data = realloc(list.data, list.size *= 2);
    list.capacity *= 2;

    if (list.data == NULL) {
      printf("Error: memory reallocation failed.\n");
      for (int i = 0; i < list.size; i++) {
        free_ast(list.data[i]);
      }
      free_ast(a);
      free(list.data);
      exit(1);
    }
  }
  list.data[list.size] = a;
  list.size += 1;
}

void free_ast_list(AstList a) {
  // for (int i = 0; i < a.size+1; i++) {
  //   free_ast(a.data[i]);
  // }
  free(a.data);
}

AstList make_ast(Parser *p, Psc_Memory_pool pool) {
  AstList x = init_astlist();

  while (p->tokens.data[p->index].type != PSC_EOF) {
    append_to_astlist(x, parse_stmt(p, pool));
  }

  return x;
}

Parser init_parser(TokenList tokens) {
  Parser p;
  p.tokens = tokens;
  p.index = 0;

  return p;
}
#include "mem.c"
#include "parser.h"
#include "lex.h"
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define initial_ast_cap 15

void free_ast(Ast_T *ast) {
  if (ast->kind == AstIdent) {
    free((AstIdentifier *)ast);
  } else if (ast->kind == AstInt) {
    free((AstNumber *)ast);
  } else if (ast->kind == Expr) {
    free_ast(((AstExpr *)ast)->lhs);
    free_ast(((AstExpr *)ast)->rhs);
    free((AstExpr *)ast);
  } else if (ast->kind == AstVar){
    free_ast(((AstVariable *)ast)->value);
    free((AstVariable *)ast);
  }
}

AstIdentifier *create_astidentifier(char *value) {
  AstIdentifier *a = (AstIdentifier *) malloc(sizeof(AstIdentifier));
  a->value = value;
  a->base.kind = AstIdent;

  return a;
}

AstNumber *create_astint(char *value) {
  AstNumber *a = (AstNumber *) malloc(sizeof(AstNumber));
  a->value = atoi(value);
  a->base.kind = AstInt;

  return a;
}

AstExpr *create_astbinop(Ast_T *lhs, Ast_T *rhs, char *op) {
  AstExpr *a = (AstExpr *) malloc(sizeof(AstExpr));
  a->base.kind = Expr;
  a->lhs = lhs;
  a->rhs = rhs;
  a->op = op;

  return a;
}

AstVariable *create_astvar(char *name, Ast_T *value) {
  AstVariable *a = (AstVariable *) malloc(sizeof(AstVariable));
  a->name = name;
  a->value = value;
  return a;
}

Token parser_eat(Parser *p) {
  //assert(p->index <= p->tokens.size);
  assert(p->tokens.data[p->index].type != PSC_EOF);
  Token tk = p->tokens.data[p->index];
  p->tokens.size -= 1;
  p->index += 1;
  return tk;
};

Token expect(Parser *p, char *val, char *err) {
  Token t = parser_eat(p);
  if (strcmp(val, t.value) != 0) {
    printf("[ERROR]: %s\n", err);
    exit(1);
  }
  return t;
}

Token parser_at(Parser *p) {
  return p->tokens.data[p->index];
};

Ast_T *parse_primary_expr(Parser *p) {
  Token tk = parser_eat(p);

  switch (tk.type) {
    case Identifier:
      return (Ast_T *)create_astidentifier(tk.value);
    case Number:
      return (Ast_T *)create_astint(tk.value);
    default:
      printf("Type not handled as primary expression. '%d'\n", tk.type);
      exit(1);
  }
};

Ast_T *parse_multiplicitave_expr(Parser *p) {
  Ast_T *left = parse_primary_expr(p);

  while (strcmp(parser_at(p).value, "/") == 0 || strcmp(parser_at(p).value, "*") == 0) {
    char *operator = parser_eat(p).value;
    Ast_T *right = parse_primary_expr(p);
    
    left = (Ast_T *) create_astbinop(left, right, operator);
    return left;
  }

  return left;
};

Ast_T *parse_additive_expr(Parser *p) {
  Ast_T *left = parse_multiplicitave_expr(p);

  while (strcmp(parser_at(p).value, "+") == 0 || strcmp(parser_at(p).value, "-") == 0) {
    char *operator = parser_eat(p).value;
    Ast_T *right = parse_multiplicitave_expr(p);
    
    left = (Ast_T *) create_astbinop(left, right, operator);
    return left;
  }

  return left;
};

Ast_T *parse_stmt(Parser *p) {
  return parse_additive_expr(p);
}

Ast_T *parse_variable_declaration(Parser *p) {
  parser_eat(p);
  char *name = parser_eat(p).value;
  expect(p, "=", "Expected assignment in variable declaration.");
  Ast_T *value = parse_stmt(p);


  return (Ast_T *) create_astvar(name, value);
}

AstList init_astlist(Psc_Memory_pool *pool) {
  AstList a;
  a.data = psc_alloc(pool, sizeof(Ast_T *) * initial_ast_cap);
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
  for (int i = 0; i < a.size+1; i++) {
    free_ast(a.data[i]);
  }
}

AstList make_ast(Parser *p, Psc_Memory_pool *pool) {
  AstList x = init_astlist(pool);

  while (p->tokens.data[p->index].type != PSC_EOF) {
    append_to_astlist(x, parse_stmt(p));
  }

  return x;
}

Parser init_parser(TokenList tokens) {
  Parser p;
  p.tokens = tokens;
  p.index = 0;

  return p;
}

#ifndef Psc_lex_header
#define Psc_lex_header
#include "mem.h"
#include <stdlib.h>

typedef struct {
  enum {
    Number,
    Identifier,
    Operator,
    PSC_EOF
  }type;
  char *value;
}Token;

typedef struct {
  Token *data;
  size_t size;
  size_t capacity;
}TokenList;

typedef struct {
  char *src;
  char current;
  unsigned int index;
  size_t srclen;
  TokenList tokens;
}Lexer;

void advance(Lexer *l);
Lexer init_lexer(char *src, Psc_Memory_pool *pool);
void tokenize(Lexer *l);
#endif
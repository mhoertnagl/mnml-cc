// mnml-cc - Compiler for the mnml-vm virtual stack machine.
// Copyright (C) 2023  Mathias Hörtnagl <mathias.hoertnagl[ÄT]gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define IS(c)      (lexer.chr != EOF && lexer.chr == c)
#define IS_NOT(c)  (lexer.chr != EOF && lexer.chr != c)
#define IS_SPACE() (lexer.chr != EOF && isspace(lexer.chr))
#define IS_DIGIT() (lexer.chr != EOF && isdigit(lexer.chr))
#define IS_ALNUM() (lexer.chr != EOF && isalnum(lexer.chr))

#define TOK_CASE(c, typ) \
  case c: {              \
    token.type = typ;    \
    next();              \
    return 0;            \
  }

#define TOK2_CASE(c1, c2, typ1, typ2) \
  case c1: {                          \
    next();                           \
    if (IS(c2)) {                     \
      token.type = typ2;              \
      next();                         \
    } else {                          \
      token.type = typ1;              \
    }                                 \
    return 0;                         \
  }

Token token;
Lexer lexer;

static void update_location() {
  if (IS('\n')) {
    lexer.line++;
    lexer.col = 1;
  } else {
    lexer.col++;
  }
}

static void next() {
  lexer.chr = getc(lexer.input);
  update_location();
}

void lexer_init(FILE *input) {
  lexer.input = input;
  lexer.line = 1;
  lexer.col = 0;
  next();
}

static Keyword keywords[] = {
  { "i64",    TOK_INT_TYPE   }, { "var",    TOK_VAR        },
  { "fn",     TOK_FN         }, { "if",     TOK_IF         },
  { "else",   TOK_ELSE       }, { "continue", TOK_CONTINUE },
  { "for",    TOK_FOR        }, { "break",  TOK_BREAK      },
  { "return", TOK_RETURN     }, { "not",    TOK_NOT        },
  { "and",    TOK_AND        }, { "or",     TOK_OR         },
};

static TokenType get_keyword_token_type() {
  for (int i = 0; i < 12; i++) {
    Keyword kw = keywords[i];
    if (strcmp(token.text, kw.name) == 0) {
      return kw.type;
    }
  }
  return TOK_ID;
}

static void read_integer() {
  int p = 0;
  while (IS_DIGIT()) {
    token.text[p++] = lexer.chr;
    next();
  }
  token.text[p] = '\0';
}

static void read_identifier() {
  int p = 0;
  while (IS_ALNUM()) {
    token.text[p++] = lexer.chr;
    next();
  }
  token.text[p] = '\0';
}

int lexer_next() {
  while (1) {
    if (lexer.chr == EOF) {
      token.type = TOK_EOF;
      return EOF;
    }

    if (IS('#')) {
      while (IS_NOT('\n')) next();
      continue;
    }

    if (IS_SPACE()) {
      while (IS_SPACE()) next();
      continue;
    }

    token.line = lexer.line;
    token.col = lexer.col;

    switch (lexer.chr) {
      TOK_CASE('(', TOK_OPEN_PARENS)
      TOK_CASE(')', TOK_CLOSE_PARENS)
      TOK_CASE('{', TOK_OPEN_BRACE)
      TOK_CASE('}', TOK_CLOSE_BRACE)
      TOK_CASE(',', TOK_COMMA)
      TOK_CASE(';', TOK_SEMICOLON)
      TOK_CASE('+', TOK_PLUS)
      TOK_CASE('-', TOK_MINUS)
      TOK_CASE('*', TOK_STAR)
      TOK_CASE('/', TOK_SLASH)
      TOK2_CASE('=', '=', TOK_ASSIGN, TOK_EQ)
      TOK2_CASE('>', '=', TOK_GT, TOK_GE)
      TOK2_CASE('<', '=', TOK_LT, TOK_LE)
      TOK2_CASE('!', '=', TOK_ERROR, TOK_NE)
    }

    if (isdigit(lexer.chr)) {
      read_integer();
      token.type = TOK_INT;
      return 0;
    }

    if (isalpha(lexer.chr)) {
      read_identifier();
      token.type = get_keyword_token_type();
      return 0;
    }
  }
  return EOF;
}

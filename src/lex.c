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

#include "lex.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define TOK_CASE(c, typ) case c: token.type = typ; break;
#define TOK2_CASE(c1, c2, typ1, typ2) \
case c1: {                            \
  next();                             \
  switch (lexer.chr) {                \
  case c2: token.type = typ2; break;  \
  default: token.type = typ1; break;  \
  }                                   \
  break;                              \
}

#define TOK(typ) token.type = typ
#define TOK2(ft, typ1, typ2)   \
{                              \
  next();                      \
  switch (lexer.chr) {         \
  case ft: TOK(typ1);  break;  \
  default:  TOK(typ2); break;  \
  }                            \
  break;                       \
}

Token token;
Lexer lexer;

static void next() {
  lexer.chr = getc(lexer.input);
  lexer.col++;
}

void lexer_reset() {
  rewind(lexer.input);
  lexer.line = 1;
  lexer.col = 1;
  next();
}

static void skip_space() {
  while (lexer.chr != EOF && isspace(lexer.chr)) {
    if (lexer.chr == '\n') {
      lexer.line++;
      lexer.col = 1;
    }
    next();
  }
}

static void skip_comment() {
  if (lexer.chr == '#') {
    while (lexer.chr != EOF && lexer.chr != '\n') {
      next();
    }
  }
}

static const int kw_lengths[] = {
  3, 3, 2, 2, 4, 4, 3,
  5, 6, 3, 3, 2,
};

static const char *keywords[] = {
  "int", "var", "fn", "if", "elif", "else", "for", 
  "break", "return", "not", "and", "or",
};

// static u8 get_opcode(const char * word) {
//   for (u64 i = 0; i < 12; i++) {
//     const char *kw = keywords[i];
//     const int len = kw_lengths[i];
//     if (strncmp(word, kw, len) == 0) {
//       return i;
//     }
//   }
//   return -1;
// }

static void read_word() {
  int p = 0;
  while (lexer.chr != EOF && !isspace(lexer.chr)) {
    token.text[p++] = lexer.chr;
    next();
  }
  token.text[p] = '\0';
}

static void set_token(u8 type) {
  token.type = type;
  token.line = lexer.line;
  token.col = lexer.col;
  read_word();
}

int lexer_next_token() {
  if (lexer.chr == EOF) {
    return EOF;
  }

  while (isspace(lexer.chr) || lexer.chr == '#') {
    skip_comment();
    skip_space();
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
  case '!': {
    next();
    switch (lexer.chr) {
    case '=': TOK(TOK_NE); break;
    default:  /* ERROR */  break;
    }
    break;
  }
  }

// int
// var
// fn
// if
// elif
// else
// for 
// break
// return
// not
// and
// or

  if (isdigit(lexer.chr)) {
    TOK(TOK_INT);
    // TODO: scan number
  }
  else if (isalpha(lexer.chr)) {
    read_word();
    for (u64 i = 0; i < 12; i++) {
      if (strcmp(token.text, keywords[i]) == 0) {
        return i;
      }
    }
    // TODO: test keywords else return ID
  }
  return 0;
}

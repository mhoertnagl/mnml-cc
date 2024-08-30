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

#pragma once

#include <stdint.h>
#include <stdio.h>

#define MAX_TEXT_LEN 256

typedef enum {
  TOK_OPEN_PARENS,  // (
  TOK_CLOSE_PARENS, // )
  TOK_OPEN_BRACE,   // {
  TOK_CLOSE_BRACE,  // }
  TOK_COMMA,        // ,
  TOK_SEMICOLON,    // ;
  TOK_ASSIGN,       // =
  TOK_EQ,           // ==
  TOK_NE,           // !=
  TOK_GT,           // >
  TOK_GE,           // >=
  TOK_LT,           // <
  TOK_LE,           // <=
  TOK_PLUS,         // +
  TOK_MINUS,        // -
  TOK_STAR,         // *
  TOK_SLASH,        // /
  TOK_INT_TYPE,     // int
  TOK_VAR,          // var
  TOK_FN,           // fn
  TOK_IF,           // if
  TOK_ELSE,         // else
  TOK_FOR,          // for
  TOK_CONTINUE,     // continue
  TOK_BREAK,        // break
  TOK_RETURN,       // return
  TOK_NOT,          // not
  TOK_AND,          // and
  TOK_OR,           // or
  TOK_INT,          // Integers
  TOK_ID,           // Identifiers
  TOK_EOF,          // End of file.
  TOK_ERROR,        // Error token
} TokenType;

typedef struct {
  const char *name;
  TokenType type;
} Keyword;

typedef struct {
  TokenType type;
  int line;
  int col;
  char text[MAX_TEXT_LEN];
} Token;

typedef struct {
  FILE *input;
  int line;
  int col;
  char chr;
} Lexer;

extern Token token;
extern Lexer lexer;

void lexer_init(FILE *input);

int lexer_next();

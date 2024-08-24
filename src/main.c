// mnml-cc - Compiler for the mnml-vm virtual stack machine.
// Copyright (C) 2024  Mathias Hörtnagl <mathias.hoertnagl[ÄT]gmail.com>
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
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s input.hl output.as\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *input = fopen(argv[1], "r");
  if (input == NULL) {
    fprintf(stderr, "ERROR: Source file [%s] not found.\n", argv[1]);
    return EXIT_FAILURE;
  }

  FILE *output = fopen(argv[2], "wb");
  if (output == NULL) {
    fprintf(stderr, "ERROR: Could not access output file [%s].\n", argv[2]);
    return EXIT_FAILURE;
  }

  lexer_init(input);

  // TODO: debugging function.
  while (lexer_next() != EOF) {
    printf("%2d:%2d ", token.line, token.col);
    switch (token.type) {
    case TOK_OPEN_PARENS:
      printf("(");
      break;
    case TOK_CLOSE_PARENS:
      printf(")");
      break;
    case TOK_OPEN_BRACE:
      printf("{");
      break;
    case TOK_CLOSE_BRACE:
      printf("}");
      break;
    case TOK_COMMA:
      printf(",");
      break;
    case TOK_SEMICOLON:
      printf(";");
      break;
    case TOK_ASSIGN:
      printf("=");
      break;
    case TOK_EQ:
      printf("==");
      break;
    case TOK_NE:
      printf("!=");
      break;
    case TOK_GT:
      printf(">");
      break;
    case TOK_GE:
      printf(">=");
      break;
    case TOK_LT:
      printf("<");
      break;
    case TOK_LE:
      printf("<=");
      break;
    case TOK_PLUS:
      printf("+");
      break;
    case TOK_MINUS:
      printf("-");
      break;
    case TOK_STAR:
      printf("*");
      break;
    case TOK_SLASH:
      printf("/");
      break;
    case TOK_INT_TYPE:
      printf("int");
      break;
    case TOK_VAR:
      printf("var");
      break;
    case TOK_FN:
      printf("fn");
      break;
    case TOK_IF:
      printf("if");
      break;
    case TOK_ELIF:
      printf("elif");
      break;
    case TOK_ELSE:
      printf("else");
      break;
    case TOK_FOR:
      printf("for");
      break;
    case TOK_BREAK:
      printf("break");
      break;
    case TOK_RETURN:
      printf("return");
      break;
    case TOK_NOT:
      printf("not");
      break;
    case TOK_AND:
      printf("and");
      break;
    case TOK_OR:
      printf("or");
      break;
    case TOK_INT:
    case TOK_ID:
      printf("%s", token.text);
      break;
    case TOK_ERROR:
      break;
    }
    printf("\n");
  }

  fclose(output);
  fclose(input);
  return EXIT_SUCCESS;
}

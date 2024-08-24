#include "parser.h"
#include <stdbool.h>

Token cur;
Token nxt;

void advance() {
  lexer_next();
  cur = nxt;
  nxt = token;
}

#define not_done() (cur.type != TOK_EOF)
#define cur_is(typ) (cur.type == typ)
#define cur_is_not(typ) (cur.type != typ)
#define nxt_is(typ) (nxt.type == typ)
#define nxt_is_not(typ) (nxt.type != typ)

static bool expect(TokenType type) {
  if (cur_is(type)) {
    advance();
    return true;
  }
  // TODO: Error expected type but got cur.type
  return false;
}

static void statement() {

}

static void function() {
  Node *n = node_create();
  expect(TOK_FN);
  // TODO: Create new token each time?
  // Need to copy string because it will be overwritten.
  n->name = cur.text;
  expect(TOK_ID);
  expect(TOK_OPEN_PARENS);
  // TODO: parse params.
  expect(TOK_CLOSE_PARENS);
  if (cur_is_not(TOK_OPEN_BRACE)) {
    // TODO: parse type.
  }
  expect(TOK_OPEN_BRACE);
  while (cur_is_not(TOK_CLOSE_BRACE)) {
    statement();
  }
}

void program() {
  while (not_done()) {
    function();
  }
}
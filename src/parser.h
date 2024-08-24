#include "lexer.h"

typedef struct Node {
  int kind;
  union {
    // Function
    struct {
      const char *name;
    };

    // Binary expression.
    struct {
      Node *left;
      TokenType op;
      Node *right;
    };
  };
} Node;

Node *node_create();
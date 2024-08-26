#include "lexer.h"
#include <stdint.h>

typedef uint64_t u64;

typedef struct {

} Type;

typedef enum {
  ND_FN,
  ND_CALL,
  ND_IF,
  ND_FOR,
  ND_RET,
  ND_UNOP,
  ND_BINOP,
  ND_INT,
} NodeKind;

typedef struct Node {
  NodeKind kind;
  union {
    // Sequences of nodes.
    struct Node *next;

    // Function.
    struct {
      const char *name;
      struct Node *params;
      Type *type;
      struct Node *stmts;
    } fn;

    // Function parameter.
    struct {
      const char *name;
      Type *type;
    } param;

    // Function call.
    struct {
      const char *name;
      struct Node *args;
    } call;

    // If statement.
    struct {
      struct Node *cond;
      struct Node *cons;
      struct Node *alt;
    } ifstmt;

    // For loop statement.
    struct {
      struct Node *decl;
      struct Node *cond;
      struct Node *post;
      struct Node *stmts;
    } forstmt;

    // Return statement.
    struct {
      struct Node *expr;
    } ret;

    // Unary operator.
    struct {
      TokenType op;
      struct Node *node;
    } unop;

    // Binary operator.
    struct {
      struct Node *lhs;
      TokenType op;
      struct Node *rhs;
    } binop;

    u64 intval;
  };
} Node;

Node *parse();
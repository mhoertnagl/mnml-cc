#include "lexer.h"
#include <stdint.h>

typedef uint64_t u64;

typedef struct {

} Type;

typedef enum {
  ND_VAR_DECL,
  ND_FN_DECL,
  ND_CALL,
  ND_IF,
  ND_FOR,
  ND_CONTINUE,
  ND_BREAK,
  ND_RET,
  ND_ASSIGN,
  ND_UNOP,
  ND_BINOP,
  ND_VAR,
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
      struct Node *stmts;
      Type *type;
    } fn_decl;

    // Function parameter.
    struct {
      const char *name;
      Type *type;
    } param;

    // Variable declaration.
    struct {
      const char *name;
      struct Node *expr;
      Type *type;
    } var_decl;

    // If statement.
    struct {
      int uuid;
      struct Node *cond;
      struct Node *cons;
      struct Node *alt;
    } if_stmt;

    // For loop statement.
    struct {
      int uuid;
      struct Node *decl;
      struct Node *cond;
      struct Node *inc;
      struct Node *stmts;
    } for_stmt;

    // Loop uuid for break and continue statements.
    int uuid;

    // Return statement.
    struct {
      struct Node *expr;
    } ret;

    // Assignment statement.
    struct {
      const char *name;
      struct Node *expr;
    } assign;

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

    // Function call.
    struct {
      const char *name;
      struct Node *args;
    } fn_call;

    // Variable.
    struct {
      const char *name;
      Type *type;
    } var;

    u64 val_int;  
  };
} Node;

Node *parse();
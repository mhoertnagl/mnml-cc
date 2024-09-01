#pragma once

#include "common.h"
#include "lexer.h"
#include "types.h"

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
  // Sequences of nodes.
  struct Node *next;
  union {
    // Function.
    struct {
      cstr name;
      struct Node *params;
      u64 paramc;
      struct Node *stmts;
      Type *type;
    } fn_decl;

    // Function parameter.
    struct {
      cstr name;
      Type *type;
    } param;

    // Variable declaration.
    struct {
      cstr name;
      struct Node *expr;
      Type *type;
    } var_decl;

    // If statement.
    struct {
      // int uuid;
      struct Node *cond;
      struct Node *cons;
      struct Node *alts;
    } if_stmt;

    // For loop statement.
    struct {
      // int uuid;
      struct Node *decl;
      struct Node *cond;
      struct Node *incr;
      struct Node *stmts;
    } for_stmt;

    //// Loop uuid for break and continue statements.
    // int uuid;

    // Return statement.
    struct {
      struct Node *expr;
    } ret;

    // Assignment statement.
    struct {
      cstr name;
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
      cstr name;
      struct Node *args;
      u64 argc;
    } fn_call;

    // Variable.
    struct {
      cstr name;
      Type *type;
    } var;

    u64 val_int;  
  };
} Node;

Node *parse();
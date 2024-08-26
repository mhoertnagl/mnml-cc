#include "codegen.h"
#include "lexer.h"
#include <stdio.h>

Codegen cg;

static void gen(Node *n);

#define lbl(name)     fprintf(cg.out, "%s\n", name)
#define ins0(op)      fprintf(cg.out, "  %s\n", op)
#define ins1(op, val) fprintf(cg.out, "  %s %lu\n", op, val)
#define jal(label)    fprintf(cg.out, "  jal %s\n", label)

static void gen_seq(Node *n) {
  for (; n != NULL; n = n->next) { 
    gen(n); 
  }
}

static void gen_fn(Node *n) {
  lbl(n->fn.name);
  gen_seq(n->fn.stmts);
}

static void gen_call(Node *n) {
  gen_seq(n->call.args);
  jal(n->call.name);
}

// static void gen_unop(Node *n) {
//   gen(n->unop.node);
//   switch (n->unop.op) {
//   case TOK_MINUS: ins("")
//   }
// }

static void gen_binop(Node *n) {
  gen(n->binop.lhs);
  gen(n->binop.rhs);
  switch (n->binop.op) {
  case TOK_EQ:    ins0("equ"); break;
  case TOK_NE:    ins0("neq"); break;
  case TOK_GT:    ins0("sgt"); break;
  case TOK_GE:    ins0("sge"); break;
  case TOK_LT:    ins0("slt"); break;
  case TOK_LE:    ins0("sle"); break;
  case TOK_PLUS:  ins0("add"); break;
  case TOK_MINUS: ins0("sub"); break;
  case TOK_STAR:  ins0("mul"); break;
  case TOK_SLASH: ins0("div"); break;
  default:
    // TODO: error.
    break;
  }
}

static void gen(Node *n) {
  switch (n->kind) {
  case ND_FN:    gen_fn(n);              break;
  case ND_CALL:  gen_call(n);            break;
  // case ND_UNOP:
  case ND_BINOP: gen_binop(n);           break;
  case ND_INT:   ins1("psh", n->intval); break;
  default:
    // TODO: error; 
    break;
  }
}

void codegen_run(Node *n, FILE* out) {
  cg.out = out;
  gen(n);
}
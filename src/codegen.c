#include "codegen.h"
#include "lexer.h"
#include <stdio.h>

Codegen cg;

static void gen(Node *n);

#define lbl(name)         fprintf(cg.out, "%s:\n", name)
#define lbl1(name, uuid)  fprintf(cg.out, "%s%d:\n", name, uuid)
#define ins0(op)          fprintf(cg.out, "  %s\n", op)
#define ins1(op, val)     fprintf(cg.out, "  %s %lu\n", op, val)
#define jmp(label)        fprintf(cg.out, "  jmp %s\n", label)
#define jmp1(label, uuid) fprintf(cg.out, "  jmp %s_%d\n", label, uuid)
#define bne(label, uuid)  fprintf(cg.out, "  bne %s_%d\n", label, uuid)
#define call(label)       fprintf(cg.out, "  cal %s\n", label)

static void gen_seq(Node *n) {
  for (; n != NULL; n = n->next) { 
    gen(n); 
  }
}

static void gen_var_decl(Node *n) {
  // Get position from symbol table.
  u64 offset = 0;
  gen(n->var_decl.expr);
  ins1("var", offset);
}

static void gen_fn_decl(Node *n) {
  lbl(n->fn_decl.name);
  gen_seq(n->fn_decl.stmts);
  ins0("ret");
}

static void gen_if(Node *n) {
  int uuid = n->if_stmt.uuid;
  gen(n->if_stmt.cond);
  bne("@_if_else", uuid);
  gen_seq(n->if_stmt.cons);
  if (n->if_stmt.alt != NULL) {
    jmp1("@_if_end", uuid);
  }
  lbl1("@_if_else", uuid);
  if (n->if_stmt.alt != NULL) {
    gen_seq(n->if_stmt.alt);
  }
  lbl1("@_if_end", uuid);
}

static void gen_for(Node *n) {
  int uuid = n->for_stmt.uuid;
  if (n->for_stmt.decl != NULL) {
    gen(n->for_stmt.decl);
  }
  lbl1("@_for_loop", uuid);
  if (n->for_stmt.cond != NULL) {
    gen(n->for_stmt.cond);
    bne("@_for_end", uuid);
  }
  gen_seq(n->for_stmt.stmts);
  lbl1("@_for_inc", uuid);
  if (n->for_stmt.inc != NULL) {
    gen(n->for_stmt.inc);
  }
  jmp1("@_for_loop", uuid);
  lbl1("@_for_end", uuid);
}

static void gen_continue(Node *n) {
  jmp1("@_for_inc", n->uuid);
}

static void gen_break(Node *n) {
  jmp1("@_for_end", n->uuid);
}

static void gen_assign(Node *n) {
  // Get position from symbol table.
  u64 offset = 0;
  gen(n->var_decl.expr);
  ins1("stv", offset);
}

static void gen_ret(Node *n) {
  ins0("ret");
}

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

static void gen_fn_call(Node *n) {
  for (Node *a = n->fn_call.args; a != NULL; a = a->next) {
    gen(a);
  }
  for (Node *a = n->fn_call.args; a != NULL; a = a->next) {
    ins0("var");
  }
  call(n->fn_call.name);
}

static void gen_var(Node *n) {
  // Get position from symbol table.
  u64 offset = 0;
  ins1("ldv", offset);
}

static void gen_int(Node *n) {
  ins1("psh", n->val_int); 
}

static void gen(Node *n) {
  switch (n->kind) {
  case ND_VAR_DECL: gen_var_decl(n); break;
  case ND_FN_DECL:  gen_fn_decl(n);  break;
  case ND_IF:       gen_if(n);       break;
  case ND_FOR:      gen_for(n);      break;
  case ND_CONTINUE: gen_continue(n); break;
  case ND_BREAK:    gen_break(n);    break;
  case ND_RET:      gen_ret(n);      break;
  case ND_ASSIGN:   gen_assign(n);   break;
  case ND_BINOP:    gen_binop(n);    break;
  case ND_CALL:     gen_fn_call(n);  break;
  case ND_VAR:      gen_var(n);      break;
  case ND_INT:      gen_int(n);      break;
  default:
    // TODO: error; 
    break;
  }
}

static void gen_program(Node *n) {
  jmp("main");
  gen(n);
}

void codegen_run(Node *n, FILE* out) {
  cg.out = out;
  cg.uuid = 0;
  gen_program(n);
}
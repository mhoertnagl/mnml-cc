#include "codegen.h"
#include "lexer.h"
#include <stdio.h>

#define lbl(name)         fprintf(cg.out, "@%s:\n", name)
#define lbl1(name, uuid)  fprintf(cg.out, "@%s.%d:\n", name, uuid)
#define ins(op)           fprintf(cg.out, "  %s\n", op)
#define psh(val)          fprintf(cg.out, "  psh %lu\n", val)
#define jmp(label)        fprintf(cg.out, "  jmp &%s\n", label)
#define jmp1(label, uuid) fprintf(cg.out, "  jmp &%s.%d\n", label, uuid)
#define jez(label, uuid)  fprintf(cg.out, "  jez &%s.%d\n", label, uuid)
// #define call(label, argc) fprintf(cg.out, "  call &%s %lu\n", label, argc)
#define call(label)       fprintf(cg.out, "  call &%s\n", label)
#define ret()             fprintf(cg.out, "  ret\n")
#define pshv()            fprintf(cg.out, "  pshv\n")
#define ldv(offset)       fprintf(cg.out, "  ldv %li\n", offset)
#define stv(offset)       fprintf(cg.out, "  stv %li\n", offset)

Codegen cg;

static void gen(Node *n);

static void gen_seq(Node *n) {
  for (; n != NULL; n = n->next) { gen(n); }
}

static void gen_var_decl(Node *n) {
  gen(n->var_decl.expr);
  pshv();
}

static void gen_fn_decl(Node *n) {
  lbl(n->fn_decl.name);
  for (u64 i = 0; i < n->fn_decl.paramc; i++) { pshv(); }
  gen_seq(n->fn_decl.stmts);
  ret();
}

static void gen_if(Node *n) {
  int id     = cg.if_id++;
  Node *cond = n->if_stmt.cond;
  Node *cons = n->if_stmt.cons;
  Node *alts = n->if_stmt.alts;

  gen(cond);
  jez("if.else", id);
  gen_seq(cons);
  if (alts != NULL) { jmp1(".if.end", id); }
  lbl1("if.else", id);
  if (alts != NULL) { gen_seq(alts); }
  lbl1("if.end", id);
}

static void gen_for(Node *n) {
  int id      = cg.for_id++;
  Node *decl  = n->for_stmt.decl;
  Node *cond  = n->for_stmt.cond;
  Node *stmts = n->for_stmt.stmts;
  Node *incr  = n->for_stmt.incr;

  if (decl != NULL) { gen(decl); }
  lbl1("for.loop", id);
  if (cond != NULL) {
    gen(cond);
    jez("for.end", id);
  }
  gen_seq(stmts);
  lbl1("for.inc", id);
  if (incr != NULL) { gen(incr); }
  jmp1("for.loop", id);
  lbl1("for.end", id);
}

static void gen_continue(Node *n) {
  jmp1("for.inc", cg.for_id);
}

static void gen_break(Node *n) {
  jmp1("for.end", cg.for_id);
}

static void gen_assignment(Node *n) {
  // Get position from symbol table.
  i64 offset = 0;
  gen(n->assign.expr);
  stv(offset);
}

static void gen_ret(Node *n) {
  if (n->ret.expr != NULL) { gen(n->ret.expr); }
  ret();
}

#define BINOP_CASE(type, mnemonic) \
  case type: ins(mnemonic); break;

static void gen_binop(Node *n) {
  gen(n->binop.lhs);
  gen(n->binop.rhs);
  switch (n->binop.op) {
  BINOP_CASE( TOK_EQ,    "eq"  )
  BINOP_CASE( TOK_NE,    "ne"  )
  BINOP_CASE( TOK_GT,    "gt"  )
  BINOP_CASE( TOK_GE,    "ge"  )
  BINOP_CASE( TOK_LT,    "lt"  )
  BINOP_CASE( TOK_LE,    "le"  )
  BINOP_CASE( TOK_PLUS,  "add" )
  BINOP_CASE( TOK_MINUS, "sub" )
  BINOP_CASE( TOK_STAR,  "mul" )
  BINOP_CASE( TOK_SLASH, "div" )
  default:
    // TODO: error.
    break;
  }
}

static void gen_fn_call(Node *n) {
  // call(n->fn_call.name, n->fn_call.argc);
  call(n->fn_call.name);
}

static void gen_var_lookup(Node *n) {
  // Get position from symbol table.
  i64 offset = 0;
  ldv(offset);
}

static void gen_int(Node *n) {
  psh(n->val_int); 
}

#define ND_CASE(type, handler) \
  case type: handler(n); break;

static void gen(Node *n) { 
  switch (n->kind) {
  ND_CASE( ND_VAR_DECL, gen_var_decl   )
  ND_CASE( ND_FN_DECL,  gen_fn_decl    )
  ND_CASE( ND_IF,       gen_if         )
  ND_CASE( ND_FOR,      gen_for        )
  ND_CASE( ND_CONTINUE, gen_continue   )
  ND_CASE( ND_BREAK,    gen_break      )
  ND_CASE( ND_RET,      gen_ret        )
  ND_CASE( ND_ASSIGN,   gen_assignment )
  ND_CASE( ND_BINOP,    gen_binop      )
  ND_CASE( ND_CALL,     gen_fn_call    )
  ND_CASE( ND_VAR,      gen_var_lookup )
  ND_CASE( ND_INT,      gen_int        )
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
  cg.if_id = 0;
  cg.for_id = 0;
  gen_program(n);
}
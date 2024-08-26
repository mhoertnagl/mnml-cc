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
#define jmp1(label, uuid) fprintf(cg.out, "  jmp %s%d\n", label, uuid)
#define bra(label, uuid)  fprintf(cg.out, "  bra %s%d\n", label, uuid)
#define jal(label)        fprintf(cg.out, "  jal %s\n", label)

int codegen_next_uuid() {
  return cg.uuid++;
}

static void gen_seq(Node *n) {
  for (; n != NULL; n = n->next) { 
    gen(n); 
  }
}

static void gen_fn(Node *n) {
  lbl(n->fn.name);
  gen_seq(n->fn.stmts);
  ins0("ret");
}

static void gen_if(Node *n) {
  int uuid = codegen_next_uuid();
  gen(n->ifstmt.cond);
  // TODO: actually bne - branch if not equal;
  // ins0("not");
  bra("else", uuid);
  gen_seq(n->ifstmt.cons);
  if (n->ifstmt.alt != NULL) {
    jmp1("endif", uuid);
  }
  lbl1("else", uuid);
  if (n->ifstmt.alt != NULL) {
    gen_seq(n->ifstmt.alt);
  }
  lbl1("endif", uuid);
}

static void gen_for(Node *n) {
  int uuid = codegen_next_uuid();
  if (n->forstmt.decl != NULL) {
    gen(n->forstmt.decl);
  }
  lbl1("for", uuid);
  if (n->forstmt.cond) {
    gen(n->forstmt.cond);
    // TODO: actually bne - branch if not equal;
    // ins0("not");
    bra("endfor", uuid);
  }
  gen_seq(n->forstmt.stmts);
  if (n->forstmt.post != NULL) {
    gen(n->forstmt.post);
  }
  jmp1("for", uuid);
  lbl1("endfor", uuid);
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

static void gen_call(Node *n) {
  // TODO: Push in reverse order?
  gen_seq(n->call.args);
  jal(n->call.name);
}

static void gen_int(Node *n) {
  ins1("psh", n->intval); 
}

static void gen(Node *n) {
  switch (n->kind) {
  case ND_FN:    gen_fn(n);    break;
  case ND_IF:    gen_if(n);    break;
  case ND_FOR:   gen_for(n);   break;
  case ND_RET:   gen_ret(n);   break;
  case ND_BINOP: gen_binop(n); break;
  case ND_CALL:  gen_call(n);  break;
  case ND_INT:   gen_int(n);   break;
  default:
    // TODO: error; 
    break;
  }
}

void codegen_run(Node *n, FILE* out) {
  cg.out = out;
  cg.uuid = 0;
  gen(n);
}
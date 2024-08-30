#include "codegen.h"
#include "lexer.h"
#include <stdio.h>

Codegen cg;

static void gen(Node *n);

#define ins(op)           fprintf(cg.out, "  %s\n", op)
#define lbl(name)         fprintf(cg.out, "@%s:\n", name)
#define lbl1(name, uuid)  fprintf(cg.out, "@%s.%d:\n", name, uuid)
#define jmp(label)        fprintf(cg.out, "  jmp @%s\n", label)
#define jmp1(label, uuid) fprintf(cg.out, "  jmp @%s.%d\n", label, uuid)
#define jne(label, uuid)  fprintf(cg.out, "  jne @%s.%d\n", label, uuid)
// #define call(label)       fprintf(cg.out, "  call @%s\n", label)
#define call(label, argc) fprintf(cg.out, "  call @%s %lu\n", label, argc)
#define ret()             fprintf(cg.out, "  ret\n")
#define psh(val)          fprintf(cg.out, "  psh %lu\n", val)
#define pshv()            fprintf(cg.out, "  pshv\n")
#define ldv(offset)       fprintf(cg.out, "  ldv %li\n", offset)
#define stv(offset)       fprintf(cg.out, "  stv %li\n", offset)

static void gen_seq(Node *n) {
  for (; n != NULL; n = n->next) { gen(n); }
}

static void gen_var_decl(Node *n) {
  gen(n->var_decl.expr);
  pshv();
}

static void gen_fn_decl(Node *n) {
  lbl(n->fn_decl.name);
  gen_seq(n->fn_decl.stmts);
  ret();
}

static void gen_if(Node *n) {
  int uuid = n->if_stmt.uuid;
  gen(n->if_stmt.cond);
  jne(".if.else", uuid);
  gen_seq(n->if_stmt.cons);
  if (n->if_stmt.alt != NULL) {
    jmp1(".if.end", uuid);
  }
  lbl1(".if.else", uuid);
  if (n->if_stmt.alt != NULL) {
    gen_seq(n->if_stmt.alt);
  }
  lbl1(".if.end", uuid);
}

static void gen_for(Node *n) {
  int uuid = n->for_stmt.uuid;
  if (n->for_stmt.decl != NULL) {
    gen(n->for_stmt.decl);
  }
  lbl1(".for.loop", uuid);
  if (n->for_stmt.cond != NULL) {
    gen(n->for_stmt.cond);
    jne(".for.end", uuid);
  }
  gen_seq(n->for_stmt.stmts);
  lbl1(".for.inc", uuid);
  if (n->for_stmt.inc != NULL) {
    gen(n->for_stmt.inc);
  }
  jmp1(".for.loop", uuid);
  lbl1(".for.end", uuid);
}

static void gen_continue(Node *n) {
  jmp1(".for.inc", n->uuid);
}

static void gen_break(Node *n) {
  jmp1(".for.end", n->uuid);
}

static void gen_assign(Node *n) {
  // Get position from symbol table.
  i64 offset = 0;
  gen(n->var_decl.expr);
  stv(offset);
}

static void gen_ret(Node *n) {
  if (n->ret.expr) {
    gen(n->ret.expr);
  }
  ret();
}

static void gen_binop(Node *n) {
  gen(n->binop.lhs);
  gen(n->binop.rhs);
  switch (n->binop.op) {
  case TOK_EQ:    ins("eq"); break;
  case TOK_NE:    ins("ne"); break;
  case TOK_GT:    ins("gt"); break;
  case TOK_GE:    ins("ge"); break;
  case TOK_LT:    ins("lt"); break;
  case TOK_LE:    ins("le"); break;
  case TOK_PLUS:  ins("add"); break;
  case TOK_MINUS: ins("sub"); break;
  case TOK_STAR:  ins("mul"); break;
  case TOK_SLASH: ins("div"); break;
  default:
    // TODO: error.
    break;
  }
}

// #define FORALL_ARGS(body) \
//   for (Node *a = n->fn_call.args; a != NULL; a = a->next) body

// static void gen_fn_call(Node *n) {
//   FORALL_ARGS({ gen(a); })
//   FORALL_ARGS({ pshv(); })
//   call(n->fn_call.name);
// }

static void gen_fn_call(Node *n) {
  call(n->fn_call.name, n->fn_call.argc);
}

static void gen_var(Node *n) {
  // Get position from symbol table.
  i64 offset = 0;
  ldv(offset);
}

static void gen_int(Node *n) {
  psh(n->val_int); 
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
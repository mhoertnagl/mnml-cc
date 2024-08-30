#pragma once

#include "common.h"
#include "types.h"

typedef enum {
  SYM_FN,
  SYM_LOCAL,
} SymbolKind;

typedef struct Symbol {
  SymbolKind kind;
  cstr name;
  struct Symbol *next;
  union {

    // struct {
    //   u64 argc;
    // } fn;

    // Call stack offset for local variables
    // relative to the frame pointer fp.
    i64 offset;
  };
} Symbol;

typedef struct SymbolTable {
  SymbolTable *parent;
  Symbol *symbols;
} SymbolTable;

void symbol_table_add(Symbol *symbol);

Symbol *symbol_table_find(cstr name);

void symbol_table_push();

void Symbol_table_pop();
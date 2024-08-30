#pragma once

#include "common.h"
#include "parser.h"
#include <stdio.h>

typedef struct {
  FILE *out;
  int if_id;
  int for_id;
} Codegen;

void codegen_run(Node *n, FILE* out);
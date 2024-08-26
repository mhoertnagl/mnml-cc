#pragma once

#include "parser.h"
#include <stdio.h>

typedef struct {
  FILE *out;
} Codegen;

void codegen_run(Node *n, FILE* out);
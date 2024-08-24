// mnml-cc - Compiler for the mnml-vm virtual stack machine.
// Copyright (C) 2024  Mathias Hörtnagl <mathias.hoertnagl[ÄT]gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "as.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s input.cc output.as\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *input = fopen(argv[1], "r");
  if (input == NULL) {
    fprintf(stderr, "ERROR: Source file [%s] not found.\n", argv[1]);
    return EXIT_FAILURE;
  }

  FILE *output = fopen(argv[2], "wb");
  if (output == NULL) {
    fprintf(stderr, "ERROR: Could not access output file [%s].\n", argv[2]);
    return EXIT_FAILURE;
  }

  assembler_run(input, output);
  fclose(output);
  fclose(input);
  return EXIT_SUCCESS;
}

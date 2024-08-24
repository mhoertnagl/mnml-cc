# mu.h - Minimalist unit testing framework.
# Copyright (C) 2023  Mathias Hörtnagl <mathias.hoertnagl[ÄT]gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC = clang
CFLAGS = -std=c17 -g -Wall

# Binary
#------------------------------------------------------------------------------
# Binary target name.
TARGET = mnmlcc

# Binary source root directory.
SRC_DIR = ./src
# Binary build root directory.
OUT_DIR = ./build
# Subdirectory for all source object files.
OBJ_DIR = $(OUT_DIR)/obj

# Binary source library dependencies.
LIBS +=
# Binary source header file dependencies.
DEPS += -I$(SRC_DIR)

# Unit testing
#------------------------------------------------------------------------------
# Unit tests source root directory.
TST_SRC_DIR = ./test
# Unit tests build root directory.
TST_OUT_DIR = ./build/tests
# Subdirectory for all unit test object files.
TST_OBJ_DIR = $(TST_OUT_DIR)/obj

# Unit tests library dependencies.
TST_LIBS +=
# Unit tests header file dependencies.
TST_DEPS += -I$(TST_SRC_DIR)

#------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#------------------------------------------------------------------------------
INC = $(shell find $(SRC_DIR) -name '*.h')
SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.c=.o))

TST_INC = $(shell find $(TST_SRC_DIR) -name '*.h')
TST_SRC = $(shell find $(TST_SRC_DIR) -name '*.c')
# Exclude binary main object file.
NMM_OBJ = $(filter-out %main.o, $(OBJ)) 
TST_OBJ = $(patsubst $(TST_SRC_DIR)/%, $(TST_OBJ_DIR)/%, $(TST_SRC:.c=.o))
# Exclude unit test main object files.
NMM_TST_OBJ = $(filter-out %.test.o, $(TST_OBJ)) 
# Gather all unit test files. 
# All files with *.test.c are considered unit test files.
TST_TARGETS = $(patsubst $(TST_SRC_DIR)/%.test.c, $(TST_OUT_DIR)/%.test, $(TST_SRC))

# Creates the binary.
all: $(TARGET)

# Cleans the build directory then rebuilds the binary.
remake: clean all

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $(OUT_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) 
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(LIBS) $(DEPS) $< -o $@

# Run all unit tests.
test: clean $(TST_TARGETS)

$(TST_OUT_DIR)/%.test: $(NMM_OBJ) $(NMM_TST_OBJ) $(TST_OBJ_DIR)/%.test.o
	$(CC) $(CFLAGS) $(LIBS) $(TST_LIBS) $^ -o $@
	$@

$(TST_OBJ_DIR)/%.o: $(TST_SRC_DIR)/%.c $(INC) $(TST_INC)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(LIBS) $(TST_LIBS) $(DEPS) $(TST_DEPS) $< -o $@

# Full clean. 
# Removes the build directory.
clean:
	@rm -rf $(OUT_DIR)

.PHONY: clean

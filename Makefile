# Paths
SRC := src
TEST := test

# C Compiler
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11

# Test
TESTFILE := $(TEST)/test.c

# Sources
CSRC := $(shell find $(SRC) -name '*.c')
COBJ := $(CSRC:.c=.o)

# Rules
%.o: %.c:
	@echo "Compiling $<. . ."
	@$(CC) -c $(CFLAGS) -o $@ $<
	@echo "Compiled to $@."

all: test

build: $(COBJ)

test: build
	@$(CC) $(CFLAGS) $(COBJ) -o $(TEST)/a.out $(TESTFILE)
	@./$(TEST)/a.out

.PHONY: clean
clean:
	@rm $(shell find . -name '*.o')
	@echo "Directories cleaned."

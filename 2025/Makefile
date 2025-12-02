CC=gcc
CFLAGS=--std=gnu11 -Werror -Wpedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wmissing-parameter-type -Iinclude
LDLIBS=-lm
SHELL=/bin/bash

CUSTOM_SRC := $(wildcard include/*.c)
CUSTOM_OBJ := $(CUSTOM_SRC:.c=.o)

$(info CUSTOM_SRC = $(CUSTOM_SRC))
$(info CUSTOM_OBJ = $(CUSTOM_OBJ))

# Empty rule cancels out any built-in rules
% : %.c

% : %.o $(CUSTOM_OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

autolint_all:
	clang-format -i solutions/*.c include/*.c include/*.h

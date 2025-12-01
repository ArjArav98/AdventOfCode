CC=gcc
CFLAGS=--std=gnu11 -Werror -Wpedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wmissing-parameter-type
SHELL=/bin/bash

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

autolint_all:
	clang-format -i solutions/*.c

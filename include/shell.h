#pragma once

#include <stdio.h>
#include <stdlib.h>

#define shell_name "oshell"

#define shell_err(fmt, ...)                     \
    do {                                        \
        fprintf(stderr, "error: ");             \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        exit(1);                                \
    } while(0);

int shell_init(int fd[2]);

int shell_repl(int fd[2]);

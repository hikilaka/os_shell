#pragma once

#include <stdio.h>

#define shell_name "oshell"

#define shell_err(fmt, ...)                     \
    do {                                        \
        fprintf(stderr, "error: ");             \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        exit(1);                                \
    } while(0);

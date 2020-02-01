#include "ipc_codec.h"

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#define check_io()          \
    do {                    \
        if (nbytes < 0) {   \
            return 1;       \
        }                   \
    } while(0)

int ipc_write_size(int fd, size_t value) {
    ssize_t nbytes = write(fd, &value, sizeof(size_t));
    check_io();
    return 0;
}

int ipc_write_string(int fd, char* str, size_t len) {
    // write the string's length
    ssize_t nbytes = write(fd, &len, sizeof(size_t));
    check_io();

    // followed by individual bytes of the string
    // not including the null terminator.
    write(fd, str, sizeof(char) * len);
    check_io();
    return 0;
}

int ipc_read_size(int fd, size_t* value) {
    ssize_t nbytes = read(fd, value, sizeof(size_t));
    check_io();
    return 0;
}

int ipc_read_string(int fd, char** str, size_t* len) {
    ssize_t nbytes = read(fd, len, sizeof(size_t));
    check_io();

    *str = malloc(sizeof(char) * (*len + 1));

    nbytes = read(fd, *str, sizeof(char) * *len);
    check_io();
    return 0;
}

#undef check_io

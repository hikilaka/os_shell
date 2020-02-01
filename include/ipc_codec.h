#pragma once

#include <stddef.h>

int ipc_write_size(int fd, size_t value);

int ipc_write_string(int fd, char* str, size_t len);

int ipc_read_size(int fd, size_t* value);

int ipc_read_string(int fd, char** str, size_t* len);

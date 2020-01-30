#pragma once

typedef int(*user_cmd_handle)(int, char*);

struct user_cmd {
    char* name;
    user_cmd_handle handle;
};

int cmd_init(void);

int cmd_register(struct user_cmd* cmd);

int cmd_find(char* name, struct user_cmd* cmd);

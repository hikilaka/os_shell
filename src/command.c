#include "command.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integrated_commands.h"

#define DEFAULT_CMD_LIST_SZ 16

static struct user_cmd* cmd_list = NULL;
static size_t cmd_list_cnt = 0;
static size_t cmd_list_sz = DEFAULT_CMD_LIST_SZ;

int alloc_cmd_list(void) {
    if (cmd_list_cnt >= cmd_list_sz) {
        cmd_list_sz *= 2;

        void* old_ptr = realloc(cmd_list,
            sizeof(struct user_cmd) * cmd_list_sz);
        
        if (old_ptr == NULL) {
            // realloc failed.
            return 1;
        }

        // free the old list if necessary
        free(old_ptr);
    }

    return 0;
}

int cmd_init(void) {
    cmd_list = calloc(sizeof(struct user_cmd), DEFAULT_CMD_LIST_SZ);

    if (cmd_list == NULL) {
        // allocation failed
        return 1;
    }

    int error = integrated_cmds_register();

    return error;
}

int cmd_register(struct user_cmd* cmd) {
    assert(cmd != NULL);
    assert(cmd->name != NULL);
    assert(cmd->handle != NULL);

    if (cmd_list_cnt >= cmd_list_sz) {
        if (alloc_cmd_list()) {
            // resizing the cmd list failed
            return 1;
        }
    }

    printf("registering command %s @ %zu\n", cmd->name, cmd_list_cnt);
    
    memcpy(&cmd_list[cmd_list_cnt], cmd, sizeof(struct user_cmd));
    cmd_list_cnt += 1;
    return 0;
}

int cmd_find(char* name, struct user_cmd* cmd) {
    return 0;
}

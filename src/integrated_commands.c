#include "integrated_commands.h"

#include <stddef.h>
#include "command.h"

static struct user_cmd integrated_cmds[] = {
    { "environ",    env_list },
    { "cd",         dir_change },
    { "dir",        dir_list },
    { "clr",        screen_clear },
    { "echo",       screen_write },
    { "quit",       shell_exit },
    { "pause",      shell_pause },
    { "help",       print_help },
    { NULL, NULL }
};

int integrated_cmds_register(void) {
    struct user_cmd* cmd = integrated_cmds;

    while (cmd->name != NULL) {
        if (cmd_register(cmd)) {
            return 1;
        }
        cmd++;
    }

    return 0;
}

int env_list(int argc, char** argv) { return 0; }

int dir_change(int argc, char** argv) { return 0; }
int dir_list(int argc, char** argv) { return 0; }

int screen_clear(int argc, char** argv) { return 0; }
int screen_write(int argc, char** argv) { return 0; }

int shell_exit(int argc, char** argv) { return 0; }
int shell_pause(int argc, char** argv) { return 0; }

int print_help(int argc, char** argv) { return 0; }

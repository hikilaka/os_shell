#include "shell.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "command.h"

struct user_input {
    char *action;
    int argc;
    char **argv;
};

void parse_input(struct user_input* input, char* buf, size_t buf_sz);

int exec_input(struct user_input* input);

void release_input(struct user_input* input);

int shell_repl(void) {
    printf("enter 'help' for help\n\n");
    size_t input_buf_sz = sizeof(char) * 256;
    ssize_t input_read_cnt = 0;
    char input_buf[input_buf_sz];
    struct user_input input;
    
    do {
        printf("%s:%s$ ", getlogin(), getenv("PWD"));
        fflush(stdout); // printf might not immediately write without '\n'

        input_read_cnt = read(STDIN_FILENO, input_buf, input_buf_sz - 1);
        
        if (input_read_cnt > 0) {
            // strip off trailing newline & add string terminator
            input_buf[input_read_cnt - 1] = 0;
            input_read_cnt--;
        } else if (input_read_cnt == 0) {
            // read() could read 0 bytes, in which case we dont need to parse
            continue;
        } else {
            perror("error:");
            return 1;
        }

        parse_input(&input, input_buf, input_read_cnt);

        if (exec_input(&input)) {
            shell_err("unable to execute command %s\n", input.action);
            return 1;
        }

        release_input(&input);
    } while(true);

    return 0;
}

void parse_input(struct user_input* input, char* buf, size_t buf_sz) {
    size_t pos = 0;

    input->action = NULL;
    input->argc = 0;
    input->argv = NULL;

    if (buf_sz > 0) {
        buf[buf_sz] = 0;
    }

    #define set_action(length)                                  \
        do {                                                    \
            input->action = calloc(length, sizeof(char));       \
            memcpy(input->action, buf + pos, length);           \
        } while (0)

    #define append_arg(length)                                          \
        do {                                                            \
            char** new_ptr = realloc(input->argv,                       \
                sizeof(char*) * (input->argc + 1));                     \
                                                                        \
            input->argv = new_ptr;                                      \
                                                                        \
            input->argv[input->argc] = calloc(length, sizeof(char));    \
            memcpy(input->argv[input->argc], buf + pos, length);        \
                                                                        \
            input->argc += 1;                                           \
        } while(0)

    for (size_t i = 0; i < buf_sz - 1; i++) {
        if (isspace(buf[i])) {
            size_t len = i - pos;

            if (len == 0) {
                pos = i + 1;
                continue;
            }

            buf[i] = 0;
            len++; // include the null character

            if (input->action == NULL) {
                // the action has not been set in user_input
                set_action(len);
            } else {
                // add an argv to user_input
                append_arg(len);
            }

            pos = i + 1;
        }
    }

    if (input->action == NULL) {
        set_action(buf_sz - pos);
    } else {
        append_arg(buf_sz - pos);
    }

    #undef set_action
    #undef append_arg
}

int exec_input(struct user_input* input) {
    struct user_cmd* cmd;

    if (!cmd_find(input->action, &cmd)) {
        return cmd->handle(input->argc, input->argv);
    } else {
        printf("invalid command: \"%s\"\n", input->action);
    }

    return 0;
}

void release_input(struct user_input* input) {
    if (input->action != NULL) {
        free(input->action);
        input->action = NULL;
    }

    for (int i = 0; i < input->argc; i++) {
        free(input->argv[i]);
    }
    
    input->argc = 0;

    if (input->argv != NULL) {
        free(input->argv);
        input->argv = NULL;
    }
}

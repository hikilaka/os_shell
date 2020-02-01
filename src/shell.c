#include "shell.h"

#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "ipc_codec.h"

// https://suchprogramming.com/new-linux-process-c/
// https://linux.die.net/man/2/pipe
// https://linux.die.net/man/3/exec

struct user_input {
    char *action;
    int argc;
    char **argv;
};

void parse_input(struct user_input* input, char* buf, size_t buf_sz);

int exec_input(struct user_input* input);

int exec_progam(struct user_input* input, int fd[2]);

void release_input(struct user_input* input);

void shell_init(int fd[2]) {
    switch (fork()) {
        case 0: { // child process -- read from efd & exec program
            char* prog_name;
            size_t prog_name_len;
            size_t argc;
            char** argv = NULL;
    
            printf("pausing child process\n");

            if (ipc_read_string(fd[0], &prog_name, &prog_name_len)) {
                perror("unable to read program pipefd");
                exit(1);
            }
            if (ipc_read_size(fd[0], &argc)) {
                perror("unable to read argc pipefd");
                exit(1);
            }
            if (argc > 0) {
                argv = malloc(sizeof(char*) * (argc + 2));
            }

            memcpy(argv[0], prog_name, prog_name_len);

            size_t argv_len;
            for (size_t i = 1; i <= argc; i++) {
                if (ipc_read_string(fd[0], &argv[i], &argv_len)) {
                    perror("unable to read argv pipefd");
                    exit(1);
                }   
            }

            printf("read name: \"%s\"\n", prog_name);
            for (size_t i = 0; i <= argc; i++) {
                printf(" - %s\n", argv[i]);
            }

            if (execv(prog_name, argv) == -1) {
                perror("");
                return;
            }
            break;
        }
        default: { // parent, main program.
            char* pwd = getenv("PWD");
            char* shell = malloc(sizeof(char) * (strlen(pwd) + strlen(shell_name) + 2));
            sprintf(shell, "%s/%s", pwd, shell_name);
            setenv("shell", shell, 1);
            free(shell);
            break;
        }
    }
}

int shell_repl(int fd[2]) {
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
            perror("error");
            return 1;
        }

        parse_input(&input, input_buf, input_read_cnt);

        if (exec_input(&input)) {
            exec_progam(&input, fd);
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
        cmd->handle(input->argc, input->argv);
        return 0;
    }

    return 1;
}

int exec_progam(struct user_input* input, int fd[2]) {
    if (ipc_write_string(fd[1], input->action, strlen(input->action))) {
        perror("unable to write program to pipefd");
        return 1;
    }
    if (ipc_write_size(fd[1], input->argc)) {
        perror("unable to write argc to pipefd");
        return 1;
    }
    for (int i = 0; i < input->argc; i++) {
        if (ipc_write_string(fd[1], input->argv[i], strlen(input->argv[1]))) {
            perror("unable to write args to pipefd");
            return 1;
        }
    }

    printf("waiting for child to finish..\n");

    wait(NULL);

    printf("child is done\n");
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

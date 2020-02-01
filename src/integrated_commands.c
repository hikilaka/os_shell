#include "integrated_commands.h"

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "command.h"

extern char** environ;

static struct user_cmd integrated_cmds[] = {
    { "environ",    env_list },
    { "cd",         dir_change },
    { "dir",        dir_list },
    { "ls",         dir_list }, // because i dislike 'dir'
    { "clr",        screen_clear },
    { "echo",       screen_write },
    { "exit",       shell_exit },
    { "quit",       shell_exit }, // alias to quit
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

#define cmd(name) int name(int argc, char** argv)
#define ignore_args() do { (void) argc; (void) argv; } while(0)

cmd(env_list) {
    if (argc > 0) {
        printf("%s\n", getenv(argv[0]));
    } else {
        for (size_t i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
        }
    }

    return 0;
}

// https://linux.die.net/man/2/chdir
cmd(dir_change) {
    if (argc > 0) {
        chdir(argv[0]);
    
        int result = setenv("PWD", argv[0], 1);
        
        if (result) {
            perror("error: ");
        }
        return result;
    } else {
        printf("%s\n", getenv("PWD"));
        return 0;
    }
}

// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
cmd(dir_list) {
    char* path = argc > 0 ? argv[0] : getenv("PWD");

    DIR* dir = opendir(path);

    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir))) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
        return 0;
    } else {
        perror("error: ");
        return 1;
    }
}

// https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
cmd(screen_clear) {
    ignore_args();

    struct winsize win_sz;
    ioctl(0, TIOCGWINSZ, &win_sz);
    
    for (size_t i = 0; i < win_sz.ws_row; i++) {
        write(STDOUT_FILENO, "\n", sizeof(char));
    }

    return 0;
}

cmd(screen_write) {
    for (int i = 0; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));

        if (i + 1 < argc) {
            write(STDOUT_FILENO, " ", sizeof(char));
        }
    }
    write(STDOUT_FILENO, "\n", sizeof(char));
    return 0;
}

cmd(shell_exit) {
    int exit_code = 0;

    if (argc > 0) {
        exit_code = atoi(argv[0]);
    }

    exit(exit_code);
    return 0;
}

cmd(shell_pause) {
    ignore_args();
    getchar();
    return 0;
}

cmd(print_help) {
    ignore_args();
    return 0;
}

#undef cmd

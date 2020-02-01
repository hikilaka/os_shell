#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"
#include "shell.h"

int main() {
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    if (shell_init(pipefd)) {
        return 1;
    }

    if (cmd_init()) {
        // shell_err will call exit(1), no need to return.
        shell_err("unable to initialize commands\n");
    }

    return shell_repl(pipefd);
}

#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "shell.h"

int main(int argc, char* argv[]) {
    printf("loading " shell_name "..\n");

    if (cmd_init()) {
        // shell_err will call exit(1), no need to return.
        shell_err("unable to initialize commands\n");
    }

    return shell_repl();
}

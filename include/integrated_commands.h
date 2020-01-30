#pragma once

int integrated_cmds_register(void);

#define def_cmd(name) \
    int name(int, char**)

def_cmd(env_list);

def_cmd(dir_change);
def_cmd(dir_list);

def_cmd(screen_clear);
def_cmd(screen_write);

def_cmd(shell_exit);
def_cmd(shell_pause);

def_cmd(print_help);

#undef def_cmd

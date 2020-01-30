cmake_minimum_required(VERSION 3.1)

project(os_shell VERSION 0.1
    DESCRIPTION "basic command shell"
    LANGUAGES C)

add_compile_options(-Wall -Wextra -Wpedantic -g)

add_executable(oshell
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/command.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/integrated_commands.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shell.c")

target_include_directories(oshell PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_features(oshell PUBLIC c_std_11)

cmake_minimum_required(VERSION 3.15)
project(5 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 1.c)
add_executable(2 2.c)
add_executable(3 mytail.c)
add_executable(4 mystat.c file_status_printer.h)
add_executable(5 myls.c file_status_printer.h)
add_executable(6 6.c stack.h stack.c)
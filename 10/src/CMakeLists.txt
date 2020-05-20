cmake_minimum_required(VERSION 3.16)
project(10 C)

set(CMAKE_C_STANDARD 11)

add_executable(reader reader.c)
add_executable(unlinker unlinker.c)
cmake_minimum_required(VERSION 3.16)
project(8 C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "-lrt")

add_executable(server server.c connection.h)
add_executable(client client.c connection.h)


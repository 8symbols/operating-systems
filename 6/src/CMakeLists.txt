cmake_minimum_required(VERSION 3.16)
project(6 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 check-xor.c timer.h)
add_executable(2 check-fletcher.c timer.h)
add_executable(3 crc.c timer.h)
add_executable(create-csum create-csum.c crc-8.h)
add_executable(check-csum check-csum.c crc-8.h)
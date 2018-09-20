cmake_minimum_required(VERSION 3.10)
project(hash-table)

set(CMAKE_C_STANDARD 11)

include_directories(include/)
add_executable(hash-table src/main.c src/hash_table.c include/project/hash_table.h)

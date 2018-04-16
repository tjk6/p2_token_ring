cmake_minimum_required(VERSION 3.8)
project(p2_token_ring C)

set(CMAKE_C_STANDARD 99)

add_executable(p2_token_ring server.c server.h)
add_executable(bbhost ring_client.c ring_client.h)
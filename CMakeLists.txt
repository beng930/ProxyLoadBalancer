cmake_minimum_required(VERSION 3.9)
project(CLionProj C)

set(CMAKE_C_STANDARD 99)

add_executable(CLionProj main.c ClientConnections.c ClientConnections.h ServerConnections.c ServerConnections.h PriorityQueue.c PriorityQueue.h lb.c)
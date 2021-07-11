cmake_minimum_required(VERSION 3.19)
project(snake)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -v")

add_link_options("-lncurses")

add_executable(snake main.cpp SnakeGame.cpp SnakeGame.h)

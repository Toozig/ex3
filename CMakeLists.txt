cmake_minimum_required(VERSION 3.12)
project(ex3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex3 main.c stack.c stack.h Parser.c Parser.h postfixEvaluation.c postfixEvaluation.h)
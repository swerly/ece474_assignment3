cmake_minimum_required(VERSION 3.6)
project(ece474_assignment3)

set( CMAKE_C_FLAGS "-Wall -g" )

set(SOURCE_FILES main.c)
add_executable(ece474_assignment3 ${SOURCE_FILES})
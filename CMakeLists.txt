cmake_minimum_required(VERSION 3.19)
project(clipboard)

set(CMAKE_C_STANDARD 11)

add_executable(clipboard WIN32 main.c resource.rc)
set_target_properties(clipboard PROPERTIES RESOURCE me.ico)
cmake_minimum_required(VERSION 3.10)
project(JTCL C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

include_directories(include)
file(GLOB SOURCES src/*.c)
add_executable(jtcl ${SOURCES})

# target_link_libraries(my_program argp)


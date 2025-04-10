cmake_minimum_required(VERSION 3.10)
project(JTCL C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
set(SOURCES
	src/main.c
	src/fs.c
	src/tcat.c
)

add_executable(jcatl ${SOURCES})

# target_link_libraries(my_program argp)


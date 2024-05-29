cmake_minimum_required(VERSION 3.13)
project(skps_projekt VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(master
    src/master.c src/server.c src/mcp3422.c
)


add_executable(slave
	src/slave.cpp
)

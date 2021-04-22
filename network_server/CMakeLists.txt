cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
project(sample)

find_package( Threads )

add_executable(
    server.o 
    server.cpp
)

target_link_libraries( server.o ${CMAKE_THREAD_LIBS_INIT} )

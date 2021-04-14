cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
project(sample)

find_package( Threads )

add_executable(
    tester
    socket_state.cpp
    networking_main.cpp
)

target_link_libraries( tester ${CMAKE_THREAD_LIBS_INIT} )
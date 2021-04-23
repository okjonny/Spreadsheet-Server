#cmake_minimum_required(VERSION 3.19)
#project(TeamPog_CS3505)
#
#set(CMAKE_CXX_STANDARD 14)
#
#include_directories(network_server)
#
#add_executable(TeamPog_CS3505
#        network_server/client.cpp
#        network_server/networking.cpp

cmake_minimum_required(VERSION 3.19)

include_directories(network_server)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(SOURCE_FILES network_server/networking.cpp network_server/socket_state.cpp
        network_server/networking.h network_server/server.cpp
        network_server/server_controller.cpp network_server/server_controller.h)
project(TeamPog_CS3505)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads)
find_package(nlohmann_json 3.2.0)

add_executable(TeamPog_CS3505
        ${SOURCE_FILES}
        )
# CMakeLists.txt
#add_library(foo ...)
#target_link_libraries(TeamPog_CS3505 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(TeamPog_CS3505 ${CMAKE_THREAD_LIBS_INIT})
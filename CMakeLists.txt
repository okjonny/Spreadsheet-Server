cmake_minimum_required(VERSION 3.19)

include_directories(network_server)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(SOURCE_FILES
        network_server/networking.cpp network_server/networking.h
        network_server/socket_state.cpp
        network_server/server_controller.cpp network_server/server_controller.h
        spreadsheet/dependency_graph.cpp spreadsheet/dependency_graph.h
        #        spreadsheet/dependency_graph_tester.cpp
        spreadsheet/spreadsheet.cpp spreadsheet/spreadsheet.h
        spreadsheet/formula.cpp spreadsheet/formula.h
        spreadsheet/cell.cpp spreadsheet/cell.h

        # MAINS:
        #        Test/test.cpp
        network_server/server.cpp
        #        network_server/spreadsheet_file.cpp
        network_server/writer.cpp network_server/writer.h
        network_server/spreadsheet_file.cpp network_server/spreadsheet_file.h
        network_server/json_struct.cpp)
project(spreadsheet_server)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads)
#find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(nlohmann_json REQUIRED
        CONFIG
        PATHS "/usr/lib/cmake/"
        )

add_executable(spreadsheet_server
        ${SOURCE_FILES}
        )
target_link_libraries(spreadsheet_server ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(TeamPog_CS3505 stdc++fs)
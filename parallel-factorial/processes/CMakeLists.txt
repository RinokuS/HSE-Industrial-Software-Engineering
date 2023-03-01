cmake_minimum_required(VERSION 3.16)
project(processes)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -lpthread")

set(SOURCE_FILES
        src/main.cpp)

add_executable(processes ${SOURCE_FILES})

find_package(fmt)
target_link_libraries(processes PRIVATE fmt)

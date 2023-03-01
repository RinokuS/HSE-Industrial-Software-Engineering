cmake_minimum_required(VERSION 3.16)
project(threads)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -lpthread")

set(SOURCE_FILES
        src/main.cpp)

add_executable(threads ${SOURCE_FILES})

find_package(fmt)
target_link_libraries(threads PRIVATE fmt)

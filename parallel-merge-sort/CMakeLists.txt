cmake_minimum_required(VERSION 3.16)
project(merge-sort)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -lpthread")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

set(SOURCE_FILES
        src/main.cpp)

add_executable(merge-sort ${SOURCE_FILES})

find_package(fmt)

HEADER_DIRECTORIES(include)
target_include_directories(merge-sort PUBLIC ${include})
target_link_libraries(merge-sort PRIVATE fmt)

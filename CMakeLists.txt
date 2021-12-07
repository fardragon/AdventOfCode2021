cmake_minimum_required(VERSION 3.21)
project(AdventOfCode2021)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(shared_lib OBJECT shared/shared.cpp)
target_include_directories(shared_lib PUBLIC shared)

set (DAYS 7)
foreach (index RANGE 1 ${DAYS})
    add_subdirectory("day${index}")
endforeach()

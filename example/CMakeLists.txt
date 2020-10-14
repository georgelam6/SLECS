cmake_minimum_required(VERSION 3.10)

project(ECSExample)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

link_directories(../bin)

add_executable(ECSExample main.cpp)
target_include_directories(ECSExample PRIVATE ../include)
target_link_libraries(ECSExample PRIVATE slecs)

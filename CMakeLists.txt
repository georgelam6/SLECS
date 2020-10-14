cmake_minimum_required(VERSION 3.10)

project(slecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)

add_subdirectory(example)

set(SOURCE
	src/EntityManager.cpp
	src/SystemManager.cpp
)

set(HEADERS
	src/ComponentArray.h
	src/ComponentManager.h
	src/ECSCommon.h
	src/EntityManager.h
	src/System.h
	src/SystemManager.h
)

add_library(slecs STATIC ${SOURCE} ${HEADERS})

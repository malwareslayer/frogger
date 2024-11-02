cmake_minimum_required(VERSION 3.20)

project(frogger VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE HEADERS "${SRC_DIR}/*.hpp")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
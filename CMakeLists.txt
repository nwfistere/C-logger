cmake_minimum_required(VERSION 3.12)

project(LOGGER)

set (CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME LoggerTest COMMAND logTest)
cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -std=gnu++2a")

project(LOGGER)

OPTION(TEST_COVERAGE "Enable test coverage" OFF)


# add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME LoggerTest COMMAND logger_test)
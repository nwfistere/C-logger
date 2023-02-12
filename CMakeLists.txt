cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_STANDARD 20)

project(LOGGER)

OPTION(TEST_COVERAGE "Enable test coverage" OFF)


# add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME LoggerTest COMMAND logger_test)
cmake_minimum_required(VERSION 3.16)


project(LOGGER)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME LoggerTest COMMAND Test)
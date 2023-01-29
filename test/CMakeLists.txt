add_executable(logTest test.cpp)

target_include_directories(logTest PRIVATE ${LOGGER_SOURCE_DIR}/src)

target_link_libraries(logTest Logger)
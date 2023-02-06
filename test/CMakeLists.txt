
add_executable(logger_test logger_test.cpp)
add_executable(logger_group_test logger_group_test.cpp)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

target_include_directories(logger_test PRIVATE ${LOGGER_SOURCE_DIR}/src)
target_include_directories(logger_group_test PRIVATE ${LOGGER_SOURCE_DIR}/src)


target_link_libraries(logger_test
    Boost::unit_test_framework
)

target_link_libraries(logger_group_test
    Boost::unit_test_framework
)

if (TEST_COVERAGE)
    SET(GCC_COVERAGE_COMPILE_FLAGS -fprofile-arcs -ftest-coverage)
    target_compile_options(logger_test PUBLIC ${GCC_COVERAGE_COMPILE_FLAGS})
    target_link_options(logger_test PUBLIC -lgcov --coverage)
    target_compile_options(logger_group_test PUBLIC ${GCC_COVERAGE_COMPILE_FLAGS})
    target_link_options(logger_group_test PUBLIC -lgcov --coverage)
endif(TEST_COVERAGE)
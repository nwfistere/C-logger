
add_executable(logger_test test_main.cpp logger_test.cpp logger_group_test.cpp)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

target_include_directories(logger_test PRIVATE ${LOGGER_SOURCE_DIR}/src)


target_link_libraries(logger_test
    Boost::unit_test_framework
)

if (TEST_COVERAGE)
    SET(GCC_COVERAGE_COMPILE_FLAGS -fprofile-arcs -ftest-coverage)
    SET(GCC_COVERAGE_LINK_FLAGS -lgcov --coverage)

    target_compile_options(logger_test PUBLIC ${GCC_COVERAGE_COMPILE_FLAGS})
    target_link_options(logger_test PUBLIC ${GCC_COVERAGE_LINK_FLAGS})
endif(TEST_COVERAGE)
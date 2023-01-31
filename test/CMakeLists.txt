add_executable(logTest basic_test.cpp)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

target_include_directories(logTest PRIVATE ${LOGGER_SOURCE_DIR}/src)

target_link_libraries(logTest
    Boost::unit_test_framework
)
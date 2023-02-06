# How to run
mkdir build && cd build
cmake ..
cmake --build . [--target clean] [--clean-first]
make test


# How to run gcov for code coverage
1. Run build with the gcc_coverage flags enabled -DTEST_COVERAGE=ON
2. Run tests
3. lcov -c -d . -o output.info
3. lcov output.info or lcov -r output.info "/usr*"
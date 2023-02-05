# How to run
mkdir build && cd build
cmake ..
cmake --build . [--target clean] [--clean-first]
make test


# How to run gcov for code coverage
1. Run build with the gcc_coverage flags enabled
2. Run tests
3. Generate gcov files by adding .o file as input
> gcov logger_test.cpp.o
2. 
#include "logger_group.hpp"

struct fixture {
    // Treated as an afterEach() function between each unit test.
    ~fixture() { logger_group::get().clear(); }
};

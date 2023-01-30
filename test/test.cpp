#include "logger.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <string>

int main(int argc, char const *argv[])
{
    std::unique_ptr<unmovable_logger<std::ostream>> logr = std::make_unique<unmovable_logger<std::ostream>>(std::cout);

    logr.get()->log(std::string("Hello world\n"));

    return 0;
}

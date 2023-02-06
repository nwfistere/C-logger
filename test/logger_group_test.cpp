#include <boost/test/unit_test.hpp>

#include "movable_logger.hpp"
#include "unmovable_logger.hpp"
#include "logger_group.hpp"

#include "fixture.hpp"

BOOST_FIXTURE_TEST_SUITE(LoggerGroupSuite, fixture)

BOOST_AUTO_TEST_CASE(OfstreamTest)
{
    std::unique_ptr<std::ofstream> ofile = std::make_unique<std::ofstream>("./output-file1.txt", std::ios::out);
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(std::make_unique<std::ofstream>("./output-file2.txt", std::ios::out), TRACE, "%^m\n"))
    );
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(ofile, TRACE, "%^m\n"))
    );

    logger_group::get().log(DEBUG, "test\n\n\n\n");
}

BOOST_AUTO_TEST_CASE(OfstreamPlusCoutTest)
{
    std::unique_ptr<std::ofstream> ofile = std::make_unique<std::ofstream>("./output-file1.txt", std::ios::out);
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(ofile, TRACE, "%^m\n"))
    );
    logger_group::get().add_logger<std::ostream>(new unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().log(DEBUG, "test\n\n\n\n");
}

BOOST_AUTO_TEST_CASE(UnmovableRvalue)
{
    logger_group::get().add_logger<std::ostream>(unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().log(DEBUG, "UnmovableRvalue Test");
}

BOOST_AUTO_TEST_CASE(UnmovableByReference)
{
    std::unique_ptr<base_logger> plogr = std::unique_ptr<base_logger>(new unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().add_logger(plogr);

    logger_group::get().log(DEBUG, "UnmovableByReference Test");
}

BOOST_AUTO_TEST_SUITE_END()

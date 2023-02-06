#include <boost/test/unit_test.hpp>

#include <filesystem>

#include "movable_logger.hpp"
#include "unmovable_logger.hpp"
#include "logger_group.hpp"
#include "redirect_cout.hpp"

#include "fixture.hpp"

BOOST_FIXTURE_TEST_SUITE(LoggerGroupSuite, fixture)

BOOST_AUTO_TEST_CASE(OfstreamTest)
{
    std::string outputfile1 = "./output-file1.txt";
    std::string outputfile2 = "./output-file2.txt";

    std::unique_ptr<std::ofstream> ofile = std::make_unique<std::ofstream>(outputfile1, std::ios::out);
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(std::make_unique<std::ofstream>(outputfile2, std::ios::out), TRACE, "%^w - %^m\n"))
    );
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(ofile, DEBUG, "%^w - %^m\n"))
    );
    logger_group::get().who("OfstreamTest");


    logger_group::get().log(DEBUG, "debug message");
    logger_group::get().log(TRACE, "trace message");

    std::ifstream filein(outputfile1);
    std::string line;
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "OfstreamTest - debug message");
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "");
    filein.close();

    filein.open(outputfile2);
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "OfstreamTest - debug message");
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "OfstreamTest - trace message");
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "");
    filein.close();


    std::filesystem::remove(outputfile1);
    std::filesystem::remove(outputfile2);
}

BOOST_AUTO_TEST_CASE(OfstreamPlusCoutTest)
{
    redirect_cout rcout;
    std::string outputfile = "./output-file1.txt";

    std::unique_ptr<std::ofstream> ofile = std::make_unique<std::ofstream>(outputfile, std::ios::out);
    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(ofile, TRACE, "%^m\n"))
    );
    logger_group::get().add_logger<std::ostream>(new unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().log(DEBUG, "testing with cout");

    std::ifstream filein(outputfile);
    std::string line;
    // Check output file
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "testing with cout");
    std::getline(filein, line);
    BOOST_CHECK_EQUAL(line, "");
    // Check cout
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "testing with cout");
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "");
}

BOOST_AUTO_TEST_CASE(UnmovableRvalue)
{
    redirect_cout rcout;
    logger_group::get().add_logger<std::ostream>(unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().log(DEBUG, "UnmovableRvalue Test");

    std::string line;
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "UnmovableRvalue Test");
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "");
}

BOOST_AUTO_TEST_CASE(UnmovableByReference)
{
    redirect_cout rcout;
    std::unique_ptr<base_logger> plogr = std::unique_ptr<base_logger>(new unmovable_logger<std::ostream>(&std::cout, TRACE, "%^m\n"));

    logger_group::get().add_logger(plogr);

    logger_group::get().log(DEBUG, "UnmovableByReference Test");

    std::string line;
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "UnmovableByReference Test");
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "");
}

BOOST_AUTO_TEST_SUITE_END()

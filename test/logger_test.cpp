#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <chrono>

#include <unistd.h>

#include "unmovable_logger.hpp"
#include "movable_logger.hpp"

BOOST_AUTO_TEST_SUITE(LoggerObjectSuite)

BOOST_AUTO_TEST_CASE(BasicTest)
{

    unmovable_logger<std::ostream> logr(&std::cout);

    logr.log(INFO, std::string("Starting Test\n"));
}

BOOST_AUTO_TEST_CASE(StringStream)
{
    std::stringstream buf("");
    std::string test_message = "Test Message";
    unmovable_logger<std::stringstream> logr(&buf);

    logr.set_format("%^m");
    logr.log(INFO, test_message);

    BOOST_CHECK(buf.good());
    BOOST_TEST_CONTEXT(buf.str());
    BOOST_CHECK_EQUAL(buf.str(), test_message);
}

BOOST_AUTO_TEST_CASE(SetFormatTest)
{
    std::stringstream buf;
    std::string test_message = "Hello!";
    unmovable_logger<std::stringstream, std::function<pid_t(void)>, std::function<gid_t(void)>> logr(&buf);

    logr.set_format("[pid(%^0) gid(%^1)] - %^m", getpid, getgid);
    logr.log(TRACE, test_message);

    std::stringstream expected_result;
    expected_result << "[pid(" << getpid() << ") gid(" << getgid() << ")] - " << test_message;

    BOOST_CHECK(buf.good());
    BOOST_CHECK_EQUAL(buf.str(), expected_result.str());

    buf.str(""); buf.clear();
    expected_result.str(""); expected_result.clear();

    logr.set_format("%^0 %^1", getpid, getgid);
    logr.log(FATAL, "Hello!");

    expected_result << getpid() << " " << getgid();

    BOOST_CHECK(buf.good());
    BOOST_TEST_CONTEXT(expected_result.str());
    BOOST_CHECK_EQUAL(buf.str(), expected_result.str());
}

static std::string get_time() {
    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::to_string(time);
}

BOOST_AUTO_TEST_CASE(TestLogLevels)
{
    std::unique_ptr<std::stringstream> buf = std::make_unique<std::stringstream>();
    // 1675282613 - (770516) INFO: INFO message
    std::string fmt = "%^0 - (%^1) " + LEVEL_STR_STR + ": %^m";
    movable_logger<
    std::stringstream,
    std::function<std::string(void)>,
    std::function<pid_t(void)>
    >
    logr(std::move(buf),
    INFO,
    fmt,
    get_time,
    getpid);

    logr.log(TRACE, "TRACE message\n");
    logr.log(DEBUG, "DEBUG message\n");
    logr.log(INFO, "INFO message\n");
    logr.log(WARN, "WARN message\n");
    logr.log(ERROR, "ERROR message\n");
    logr.log(FATAL, "FATAL message\n");

    std::unique_ptr<std::stringstream> returned_buf = logr.get_stream();
    std::string result_string = returned_buf.get()->str();
    
    BOOST_CHECK_MESSAGE(result_string.find("TRACE") == std::string::npos, "Found TRACE in logs");
    BOOST_CHECK_MESSAGE(result_string.find("DEBUG") == std::string::npos, "Found DEBUG in logs");
    BOOST_CHECK_MESSAGE(result_string.find("INFO") != std::string::npos, "Did not find INFO in logs");
    BOOST_CHECK_MESSAGE(result_string.find("WARN") != std::string::npos, "Did not find WARN in logs");
    BOOST_CHECK_MESSAGE(result_string.find("ERROR") != std::string::npos, "Did not find ERROR in logs");
    BOOST_CHECK_MESSAGE(result_string.find("FATAL") != std::string::npos, "Did not find FATAL in logs");
}

BOOST_AUTO_TEST_SUITE_END()

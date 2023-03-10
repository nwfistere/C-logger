#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <chrono>

#include <unistd.h>

#include "unmovable_logger.hpp"
#include "movable_logger.hpp"
#include "redirect_cout.hpp"

BOOST_AUTO_TEST_SUITE(LoggerObjectSuite)

BOOST_AUTO_TEST_CASE(BasicTest) {
    redirect_cout rcout;

    unmovable_logger<std::ostream> logr(&std::cout);

    logr.log(INFO, std::string("Starting Test\n"));
    std::string line;
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "Starting Test");
    std::getline(rcout.cout_stream, line);
    BOOST_CHECK_EQUAL(line, "");
}

BOOST_AUTO_TEST_CASE(StringStream) {
    std::stringstream buf("");
    std::string test_message = "Test Message";
    unmovable_logger<std::stringstream> logr(&buf);

    logr.set_format("%^m");
    logr.log(INFO, test_message);

    BOOST_CHECK(buf.good());
    BOOST_TEST_CONTEXT(buf.str());
    BOOST_CHECK_EQUAL(buf.str(), test_message);
}

BOOST_AUTO_TEST_CASE(SetFormatTest) {
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

BOOST_AUTO_TEST_CASE(TestLogLevels) {
    std::unique_ptr<std::stringstream> buf = std::make_unique<std::stringstream>();
    std::stringstream * raw_buf = buf.get();
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

    std::string line;
    std::string prefix = get_time() + " - (" + std::to_string(getpid()) + ") ";
    std::getline(*raw_buf, line);
    BOOST_CHECK_EQUAL(line, prefix + "INFO: INFO message");
    std::getline(*raw_buf, line);
    BOOST_CHECK_EQUAL(line, prefix + "WARN: WARN message");
    std::getline(*raw_buf, line);
    BOOST_CHECK_EQUAL(line, prefix + "ERROR: ERROR message");
    std::getline(*raw_buf, line);
    BOOST_CHECK_EQUAL(line, prefix + "FATAL: FATAL message");
    std::getline(*raw_buf, line);
    BOOST_CHECK_EQUAL(line, "");

    BOOST_CHECK_EQUAL(logr.get_level(), INFO);
    logr.set_level(NONE);
    BOOST_CHECK_EQUAL(logr.get_level(), NONE);
}

BOOST_AUTO_TEST_SUITE_END()

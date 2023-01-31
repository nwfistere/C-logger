#include "basic_test.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <string>

#include <unistd.h>


BOOST_AUTO_TEST_CASE(BasicTest)
{

    unmovable_logger<std::ostream> logr(std::cout);

    logr.log(INFO, std::string("Starting Test\n"));

	// To simplify this example test, let's suppose we'll test 'float'.
	// Some test are stupid, but all should pass.
	float x = 9.5f;

	BOOST_CHECK(x != 0.0f);
	BOOST_CHECK_EQUAL((int)x, 9);
	BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}

BOOST_AUTO_TEST_CASE(StringStream)
{
    std::stringstream buf("");
    std::string test_message = "Test Message";
    unmovable_logger<std::stringstream> logr(buf);

    logr.log(INFO, test_message);

    BOOST_CHECK(buf.good());
    BOOST_CHECK(buf.str() == test_message);
}

BOOST_AUTO_TEST_CASE(SetFormatTest)
{
    std::stringstream buf("");
    std::string test_message = "Test Message";
    unmovable_logger<std::stringstream> logr(buf);

    logr.set_format<pid_t(void), gid_t(void)>(getpid, getgid);

    std::cout << buf.str() << std::endl;

    logr.set_format<pid_t(void), gid_t(void), char*(char*, size_t)>(getpid, getgid, getcwd);

    std::cout << buf.str() << std::endl;

    BOOST_CHECK(buf.good());
    // BOOST_CHECK(buf.str() == test_message);
}
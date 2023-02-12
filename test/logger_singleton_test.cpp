#include <boost/test/unit_test.hpp>

#include <chrono>
#include <ctime>

#include "movable_logger.hpp"
#include "logger_singleton_macros.hpp"


BOOST_AUTO_TEST_SUITE(LoggerSingletonSuite)

static std::string current_date() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char str[100];
    std::strftime(str, sizeof(str), "%d %m %Y", std::localtime(&now));
    return str;
}

BOOST_AUTO_TEST_CASE(SingletonTest) {
    typedef movable_logger<std::ofstream, std::function<std::string()>, std::function<std::string()>> mlogger;

    std::string outputfile = "./singleton_output.txt";

    std::unique_ptr<base_logger> logr(new mlogger (
        std::make_unique<std::ofstream>(outputfile, std::ios::out),
        TRACE,
        "(%^0) [%^1] {%^w} - %^m\n",
        current_date,
        []() {
            auto start = std::chrono::system_clock::now();
            auto legacyStart = std::chrono::system_clock::to_time_t(start);
            std::string time = std::ctime(&legacyStart);
            time.erase(std::remove(time.begin(), time.end(), '\n'), time.cend());
            return time;
        }
    ));

    logger_singleton::get().add_logger(logr);

    WHO("SingletonTest");

    INFO("TESTING TEST");
    ERROR(std::to_string(__cplusplus));
}

BOOST_AUTO_TEST_SUITE_END()

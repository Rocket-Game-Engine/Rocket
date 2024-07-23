#define BOOST_TEST_MODULE RocketGameEngine Tests
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

struct TestFixture
{
    // turns off logging temporarily
    TestFixture() {
        auto logger = spdlog::basic_logger_mt("testfilelogger", "tests/log.log", true);
        logger->set_level(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    }
};

BOOST_GLOBAL_FIXTURE(TestFixture);
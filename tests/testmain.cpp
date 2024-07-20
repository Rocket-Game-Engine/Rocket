#define BOOST_TEST_MODULE RocketGameEngine Tests
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

// LOGGER TESTS

BOOST_AUTO_TEST_SUITE( logger_tests )

BOOST_AUTO_TEST_CASE( logger_no_error )
{
    BOOST_WARN_MESSAGE( 0, "HI THERE" );
}

BOOST_AUTO_TEST_SUITE_END()

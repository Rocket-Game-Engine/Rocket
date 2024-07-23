#include <boost/test/unit_test.hpp>

#include <ECS/Roc_ECS.hpp>
#include <spdlog/spdlog.h>

BOOST_AUTO_TEST_SUITE( Component_Tests )

BOOST_AUTO_TEST_CASE( GravityComponent_Tests )
{
    // Ensure gravity property exists
    SPDLOG_TRACE("Test Gravity Properties Exist");
    Gravity g;
    BOOST_CHECK_NO_THROW( g.gravity = 8.7 );
}

BOOST_AUTO_TEST_SUITE_END()
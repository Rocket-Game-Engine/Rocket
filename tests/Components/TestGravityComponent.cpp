#include <boost/test/unit_test.hpp>

#include <ECS/Roc_ECS.hpp>

BOOST_AUTO_TEST_SUITE( Component_Tests )

BOOST_AUTO_TEST_CASE( GravityComponent_Tests )
{
    // Ensure gravity property exists
    Gravity g;
    BOOST_CHECK_NO_THROW( g.gravity = 8.7 );
}

BOOST_AUTO_TEST_SUITE_END()
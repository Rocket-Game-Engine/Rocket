#include <boost/test/unit_test.hpp>

#include <ECS/Roc_ECS.hpp>

BOOST_AUTO_TEST_SUITE( Component_Tests )

BOOST_AUTO_TEST_CASE( TransformComponent_Tests )
{
    SPDLOG_TRACE("Test Transform Properties Exist");
    // Ensure offset properties exist
    Transform t;
    BOOST_CHECK_NO_THROW( t.x = 0.4 );
    BOOST_CHECK_NO_THROW( t.y = 0.5 );
    BOOST_CHECK_NO_THROW( t.z = 0.6 );
}

BOOST_AUTO_TEST_SUITE_END()
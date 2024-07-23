#include <boost/test/unit_test.hpp>

#include <ECS/Roc_ECS.hpp>

BOOST_AUTO_TEST_SUITE( Component_Tests )

BOOST_AUTO_TEST_CASE( RectangleColliderComponent_Tests )
{
    SPDLOG_TRACE("Test RectangleCollider Properties Exist");
    // Ensure offset properties exist
    RectangleCollider rc;
    BOOST_CHECK_NO_THROW( rc.offsetX = 0.5 );
    BOOST_CHECK_NO_THROW( rc.offsetY = 0.5 );

    // Ensure size properties exist
    BOOST_CHECK_NO_THROW( rc.width = 5.0 );
    BOOST_CHECK_NO_THROW( rc.height = 5.0 );

    // Ensure collision vector exists
    BOOST_CHECK_NO_THROW( rc.collisions.emplace_back(Collision()) );
}

BOOST_AUTO_TEST_SUITE_END()
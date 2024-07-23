#include <boost/test/unit_test.hpp>

#include <ECS/Roc_ECS.hpp>

#define EPSILON 0.0001

struct ECS_Fixture
{
    ECS_Fixture()
    {
        Coordinator* c = Coordinator::Get();
        c->Init();
        c->RegisterComponent<Transform>();
        c->RegisterComponent<Gravity>();
        c->CreateEntity("test_ent");
        c->CreateEntity("test_entity2");

        c->AddComponent<Gravity>(c->GetEntity("test_ent"), Gravity());
    }
    ~ECS_Fixture()
    {
        Coordinator::DeleteCoordinator();
    }
};

// LOGGER TESTS

BOOST_AUTO_TEST_SUITE( ECS_Tests )

// Sanity tests Entity-related features of the ECS
BOOST_FIXTURE_TEST_CASE( EntityCommands_Tests, ECS_Fixture )
{
    // Do real entities show up with the correct values?
    SPDLOG_TRACE("Test Real Entities Exist");
    Coordinator* c = Coordinator::Get();
    BOOST_TEST( c->GetEntity("test_ent") == 0 );
    BOOST_TEST( c->GetEntity("test_entity2") == 1 );

    // Do fake entities return MAX_ENTITIES?
    SPDLOG_TRACE("Test Fake Entities Don't Exist");
    BOOST_TEST( c->GetEntity("DNE") == MAX_ENTITIES );

    // Does destroying a real entitiy actually destroy it?
    SPDLOG_TRACE("Test Destroy Real Entity");
    c->DestroyEntity( "test_entity2" );
    BOOST_TEST( c->GetEntity("test_entity2") == MAX_ENTITIES );

    // Does destroying a fake entity allow runtime to continue?
    SPDLOG_TRACE("Test Destroy Fake Entity");
    c->DestroyEntity( "DNE" );

    // Does creating a new entity return the expected entity id?
    SPDLOG_TRACE("Test New Entity ID Num Correct");
    BOOST_TEST( c->CreateEntity("test_entity3") == 2 );

    // Does deleting all entities actually delete all entities?
    SPDLOG_TRACE("Test Deleting All Entities");
    c->DestroyAllEntities();
    BOOST_TEST( c->GetEntity("test_ent") == MAX_ENTITIES );
    BOOST_TEST( c->GetEntity("test_entity3") == MAX_ENTITIES );
}


// Sanity tests Component-related features of the ECS
BOOST_FIXTURE_TEST_CASE( ComponentCommands_Tests, ECS_Fixture )
{
    // Sanity check, do all entities exist still?
    SPDLOG_TRACE("Sanity Check - All Entities Exist (ComponentCommands)");
    Coordinator* c = Coordinator::Get();
    BOOST_TEST( c->GetEntity("test_ent") == 0 );
    BOOST_TEST( c->GetEntity("test_entity2") == 1 );

    // Does getting a component from an entity get the correct thing?
    SPDLOG_TRACE("Test Get Component from Entity with Component");
    Gravity& g = c->GetComponent<Gravity>(c->GetEntity("test_ent"));
    BOOST_TEST( fabs(g.gravity - 9.81) < EPSILON );

    // Does getting a component from an entity that doesn't exist fail?
    SPDLOG_TRACE("Test Get Component from Entity that DNE");
    BOOST_CHECK_THROW( c->GetComponent<Gravity>(c->GetEntity("test_entity2")),
                       std::runtime_error );

    // Does adding a component that doesn't exist (to any entity, real or not)
    // result in a fail?
    SPDLOG_TRACE("Test Add Unloaded Component to any Entity");
    BOOST_CHECK_THROW( c->AddComponent<RectangleCollider>(c->GetEntity("test_ent"), RectangleCollider()),
                       std::runtime_error );
    BOOST_CHECK_THROW( c->AddComponent<RectangleCollider>(c->GetEntity("DNE"), RectangleCollider()),
                       std::runtime_error );

    // Does removing a component from an entity actually remove it?
    SPDLOG_TRACE("Test Reove Component from Real Entity");
    BOOST_TEST( c->RemoveComponent<Gravity>(c->GetEntity("test_ent")) );
    BOOST_CHECK_THROW( c->GetComponent<Gravity>(c->GetEntity("test_ent")),
                       std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( SystemCommands_Tests, ECS_Fixture )
{
    // Sanity check, do all entities exist still?
    SPDLOG_TRACE("Sanity Check - All Entities Exist (SystemCommands)");
    Coordinator* c = Coordinator::Get();
    BOOST_TEST( c->GetEntity("test_ent") == 0 );
    BOOST_TEST( c->GetEntity("test_entity2") == 1 );

    // Startup check - register RectangleCollider for CollisionSystem
    SPDLOG_TRACE("Registering RectangleCollider for tests");
    BOOST_TEST( c->RegisterComponent<RectangleCollider>() );


    // Does registering a System return a shared ptr to the system?
    SPDLOG_TRACE("Test Get Registered System");
    auto sysptr = c->RegisterSystem<CollisionSystem>();
    BOOST_CHECK_NO_THROW( sysptr->GetSignature() );

    // Does adding a necessary component to an entity add it to mEntities?
    SPDLOG_TRACE("Test Adding Components to Entity s.t. it's added to System");
    BOOST_CHECK( c->GetSystem<CollisionSystem>()->mEntities.size() == 0 );
    BOOST_CHECK_NO_THROW( c->AddComponent<RectangleCollider>(c->GetEntity("test_ent"), RectangleCollider()) );
    BOOST_CHECK_NO_THROW( c->AddComponent<Transform>(c->GetEntity("test_ent"), Transform()) );
    BOOST_CHECK( c->GetSystem<CollisionSystem>()->mEntities.size() == 1 );
}



BOOST_AUTO_TEST_SUITE_END()

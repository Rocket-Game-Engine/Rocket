#include <boost/test/unit_test.hpp>
// #include <boost/test/execution_monitor.hpp>

#include <ECS/Roc_ECS.hpp>

#define EPSILON 0.0001

struct ECS_Fixture
{
    ECS_Fixture()
    {
        Coordinator* c = Coordinator::Get();
        c->Init();
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

BOOST_AUTO_TEST_SUITE( ecs_tests )

// Sanity tests Entity-related features of the ECS
BOOST_FIXTURE_TEST_CASE( test_entity_commands, ECS_Fixture )
{
    // Do real entities show up with the correct values?
    Coordinator* c = Coordinator::Get();
    BOOST_TEST( c->GetEntity("test_ent") == 0 );
    BOOST_TEST( c->GetEntity("test_entity2") == 1 );

    // Do fake entities return MAX_ENTITIES?
    BOOST_TEST( c->GetEntity("DNE") == MAX_ENTITIES );

    // Does destroying a real entitiy actually destroy it?
    c->DestroyEntity( "test_entity2" );
    BOOST_TEST( c->GetEntity("test_entity2") == MAX_ENTITIES );

    // Does destroying a fake entity allow runtime to continue?
    c->DestroyEntity( "DNE" );

    // Does creating a new entity return the expected entity id?
    BOOST_TEST( c->CreateEntity("test_entity3") == 2 );

    // Does deleting all entities actually delete all entities?
    c->DestroyAllEntities();
    BOOST_TEST( c->GetEntity("test_ent") == MAX_ENTITIES );
    BOOST_TEST( c->GetEntity("test_entity3") == MAX_ENTITIES );
}


// Sanity tests Component-related features of the ECS
BOOST_FIXTURE_TEST_CASE( test_component_commands, ECS_Fixture )
{
    // Sanity check, do all entities exist still?
    Coordinator* c = Coordinator::Get();
    BOOST_TEST( c->GetEntity("test_ent") == 0 );
    BOOST_TEST( c->GetEntity("test_entity2") == 1 );

    // Does getting a component from an entity get the correct thing?
    Gravity& g = c->GetComponent<Gravity>(c->GetEntity("test_ent"));
    BOOST_TEST( fabs(g.gravity - 9.81) < EPSILON );

    // Does getting a component from an entity that doesn't exist fail?
    BOOST_REQUIRE_THROW( c->GetComponent<Gravity>(c->GetEntity("test_entity2")),
                         std::runtime_error );
}



BOOST_AUTO_TEST_SUITE_END()

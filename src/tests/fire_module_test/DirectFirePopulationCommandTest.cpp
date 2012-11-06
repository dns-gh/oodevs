// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"
#include "module_tester/MakeModel.h"
#include <boost/bind/apply.hpp>

namespace
{
    struct FireFixture : sword::fire::ModuleFixture
    {
        FireFixture()
            : population( model[ "populations" ][ 53 ] )
        {
            ExpectCallback( 4 );
            command = StartCommand( "direct fire population",
                                    core::MakeModel( "action", 117 )
                                                   ( "identifier", 42 )
                                                   ( "population", 53 )
                                                   ( "percentage", 1 )
                                                   ( "mode", 0 )
                                                   ( "dotation", ammo_1 ) );
            mock::verify();
        }
        ~FireFixture()
        {
            ExpectCallback( 5 );
            ExpectEvent( "direct fire population", sword::test::MakeModel( "entity/identifier", 42 )
                                                                         ( "running", false ) );
            StopCommand( command );
        }
        void ExpectCallback( int code ) // $$$$ MCO 2012-04-27: use RoleAction_DirectFiring::E_ReturnCode ?
        {
            ExpectEvent( "direct fire population callback", sword::test::MakeModel( "entity", 42 )( "action", 117 )( "code", code ) );
        }
        core::Model& population;
        std::size_t command;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_impossible_if_target_knowledge_is_invalid, FireFixture )
{
    MOCK_EXPECT( IsPopulationKnowledgeValid ).once().with( core::Convert( &entity ), sword::test::MakeModel( 53 ) ).returns( false );
    ExpectCallback( 0 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_enemy_destroyed_if_no_population_element_is_found, FireFixture )
{
    MOCK_EXPECT( IsPopulationKnowledgeValid ).once().with( core::Convert( &entity ), sword::test::MakeModel( 53 ) ).returns( true );
    MOCK_EXPECT( GetClosestAlivePopulationElement ).once().with( core::Convert( &model ), sword::test::MakeModel( 53 ), firer ).returns( 0 );
    ExpectCallback( 1 );
    ExecuteCommands();
}

namespace
{
    struct PopulationElementFixture : FireFixture
    {
        PopulationElementFixture()
        {
            MOCK_EXPECT( IsPopulationKnowledgeValid ).with( core::Convert( &entity ), sword::test::MakeModel( 53 ) ).returns( true );
            MOCK_EXPECT( GetClosestAlivePopulationElement ).with( core::Convert( &model ), sword::test::MakeModel( 53 ), firer ).returns( core::Convert( &element ) );
        }
        core::Model element;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_temporary_blocked_when_blocked_with_default_coefficient, PopulationElementFixture )
{
    MOCK_EXPECT( IsTemporarilyBlocked ).once().with( firer, 100u ).returns( true );
    ExpectCallback( 6 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_temporary_blocked_when_blocked_with_custom_coefficient, PopulationElementFixture )
{
    InitializeDecisional( "<decisional>"
                          "  <urban-combat hit-factor='42'/>"
                          "  <force-ratio default-feedback-time='600s'/>"
                          "</decisional>", 10 );
    MOCK_EXPECT( IsTemporarilyBlocked ).once().with( firer, 42u ).returns( true );
    ExpectCallback( 6 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_no_capacity_if_target_has_no_component, PopulationElementFixture )
{
    MOCK_EXPECT( IsTemporarilyBlocked ).once().returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_no_capacity_if_firer_has_no_weapon, PopulationElementFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ];
    MOCK_EXPECT( IsTemporarilyBlocked ).once().returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

namespace
{
    struct WeaponFixture : PopulationElementFixture
    {
        WeaponFixture()
            : component_2( entity[ "components" ].AddElement() )
            , weapon   ( component_2[ "weapons" ].AddElement() )
        {
            component_2[ "volume" ] = volume_1;
            component_2[ "component_2" ].SetUserData( &component_2 );
            weapon[ "type" ] = "launcher_1/ammo_1";
            MOCK_EXPECT( IsTemporarilyBlocked ).returns( false );
        }
        core::Model& component_2;
        core::Model& weapon;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_no_capacity_if_firer_cannot_fire, WeaponFixture )
{
    MOCK_EXPECT( CanFire ).returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_no_ammunition_when_out_of_dotation, WeaponFixture )
{
    MOCK_EXPECT( CanFire ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    ExpectCallback( 3 );
    ExecuteCommands();
}

namespace
{
    struct FiringFixture : WeaponFixture
    {
        FiringFixture()
        {
            MOCK_EXPECT( CanFire ).returns( true );
            MOCK_EXPECT( HasDotation ).returns( true );
            model[ "tick" ] = 0;
            element[ "data" ].SetUserData( &data );
        }
        int data;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_running_and_hits, FiringFixture )
{
    MOCK_EXPECT( ComputeKilledHumans ).once().with( firer, core::Convert( &element ) ).returns( 3 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3u ).returns( 2u ); // $$$$ MCO 2012-09-21: for some reason we fire 1 round per killed human ?
    MOCK_EXPECT( GetPopulationElementPh ).once().with( firer, core::Convert( &element ) ).returns( 0.9 );
    MOCK_EXPECT( GetFireRandomNumber ).exactly( 2 ).returns( 0 ); // $$$$ MCO 2012-09-21: one random per reserved ammo
    ExpectCallback( 4 );
    ExpectEvent( "direct fire population attack",
        sword::test::MakeModel( "entity", sword::test::MakeModel( "identifier", 42 )
                                                                ( "components", mock::any )
                                                                ( "knowledges", mock::any )
                                                                ( "data", mock::any ) ) // $$$$ MCO 2012-09-21: this is actually a copy of 'entity'
                              ( "population", 53 )
                              ( "element/data", sword::test::MakeUserData( &data ) )
                              ( "paused", false ) );
    ExpectEvent( "direct fire population",
        sword::test::MakeModel( "entity/identifier", 42 )
                              ( "entity/data", "data" )
                              ( "element/data", sword::test::MakeUserData( &data ) )
                              ( "dotation", ammo_1 )
                              ( "hits", 2 )
                              ( "running", true ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_population_command_reports_running_and_no_hit_when_weapon_not_ready, FiringFixture )
{
    MOCK_EXPECT( ComputeKilledHumans ).returns( 3 );
    MOCK_EXPECT( ReserveAmmunition ).returns( 2 );
    MOCK_EXPECT( GetPopulationElementPh ).returns( 0.9 );
    MOCK_EXPECT( GetFireRandomNumber ).returns( 0 );
    ExpectCallback( 4 );
    ExpectEvent( "direct fire population attack",
        sword::test::MakeModel( "entity", sword::test::MakeModel( "identifier", 42 )
                                                                ( "components", mock::any )
                                                                ( "knowledges", mock::any )
                                                                ( "data", mock::any ) ) // $$$$ MCO 2012-09-21: this is actually a copy of 'entity'
                               ( "population", 53 )
                               ( "element/data", sword::test::MakeUserData( &data ) )
                               ( "paused", false ) );
    ExpectEvent( "direct fire population",
        sword::test::MakeModel( "entity/identifier", 42 )
                              ( "entity/data", "data" )
                              ( "element/data", sword::test::MakeUserData( &data ) )
                              ( "dotation", ammo_1 )
                              ( "hits", 2 )
                              ( "running", true ) );
    ExpectCallback( 4 );
    const std::size_t command = StartCommand( "direct fire population",
                                               core::MakeModel( "action", 117 )
                                                              ( "identifier", 42 )
                                                              ( "population", 53 )
                                                              ( "percentage", 1 )
                                                              ( "mode", 0 )
                                                              ( "dotation", ammo_1 ) );
    ExpectCallback( 4 );

    ExecuteCommands();

    ExpectCallback( 5 );
    ExpectEvent( "direct fire population", sword::test::MakeModel( "entity/identifier", 42 )
                                                                 ( "running", false ) );
    StopCommand( command );
}

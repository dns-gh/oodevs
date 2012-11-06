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
        {
            ExpectCallback( 4 );
            command = StartCommand( "direct fire",
                                    core::MakeModel( "action", 117 )
                                                   ( "identifier", 42 )
                                                   ( "enemy", 51 )
                                                   ( "percentage", 0.07 )
                                                   ( "mode", 0 )
                                                   ( "type", 0 )
                                                   ( "major", false )
                                                   ( "dotation", 0 ) );
            mock::verify();
        }
        virtual ~FireFixture()
        {
            ExpectCallback( 5 );
            ExpectEvent( "direct fire pion", sword::test::MakeModel( "entity/identifier", 42 )
                                                                   ( "running", false ) );
            StopCommand( command );
        }
        void ExpectCallback( int code ) // $$$$ MCO 2012-04-27: use RoleAction_DirectFiring::E_ReturnCode ?
        {
            ExpectEvent( "direct fire pion callback", sword::test::MakeModel( "entity", 42 )
                                                                            ( "action", 117 )
                                                                            ( "code", code ) );
        }
        std::size_t command;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_impossible_if_target_knowledge_is_invalid, FireFixture )
{
    MOCK_EXPECT( IsAgentKnowledgeValid ).once().with( enemy ).returns( false );
    ExpectCallback( 0 );
    ExecuteCommands();
}

namespace
{
    struct ValidFireFixture : FireFixture
    {
        ValidFireFixture()
        {
            MOCK_EXPECT( IsAgentKnowledgeValid ).returns( true );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_enemy_destroyed_if_target_is_dead, ValidFireFixture )
{
    target[ "dead" ] = true;
    ExpectCallback( 1 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_temporary_blocked_when_blocked_with_default_coefficient, ValidFireFixture )
{
    MOCK_EXPECT( IsTemporarilyBlocked ).once().with( firer, 100u ).returns( true );
    ExpectCallback( 6 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_temporary_blocked_when_blocked_with_custom_coefficient, ValidFireFixture )
{
    InitializeDecisional( "<decisional>"
                          "  <urban-combat hit-factor='42'/>"
                          "  <force-ratio default-feedback-time='600s'/>"
                          "</decisional>", 10 );
    MOCK_EXPECT( IsTemporarilyBlocked ).once().with( firer, 42u ).returns( true );
    ExpectCallback( 6 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_target_has_no_component, ValidFireFixture )
{
    MOCK_EXPECT( IsTemporarilyBlocked ).once().returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_firer_has_no_weapon, ValidFireFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ];
    MOCK_EXPECT( IsTemporarilyBlocked ).once().returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

namespace
{
    struct WeaponFixture : ValidFireFixture
    {
        WeaponFixture()
            : component_2( entity[ "components" ].AddElement() )
            , weapon     ( component_2[ "weapons" ].AddElement() )
        {
            component_2[ "volume" ] = volume_1;
            weapon[ "type" ] = "launcher_1/ammo_1";
            MOCK_EXPECT( IsTemporarilyBlocked ).returns( false );
        }
        core::Model& component_2;
        core::Model& weapon;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_firer_cannot_fire, WeaponFixture )
{
    MOCK_EXPECT( CanFire ).returns( false );
    ExpectCallback( 2 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_ammunition_when_out_of_dotation, WeaponFixture )
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
        }
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_enemy_destroyed_when_enemy_has_no_visible_component, FiringFixture )
{
    ExpectCallback( 1 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_enemy_destroyed_when_no_component_can_be_found_to_fire_at, FiringFixture )
{
    core::Model& component_2 = model[ "entities" ][ 43 ][ "components" ].AddElement();
    MOCK_EXPECT( CanComponentBeFiredAt ).once().with( core::Convert( &component_2 ), mock::any ).returns( false );
    ExpectCallback( 1 );
    ExecuteCommands();
}

namespace
{
    struct OneAtOneFiringFixture : FiringFixture
    {
        OneAtOneFiringFixture()
            : component_1( entity[ "components" ].AddElement() )
            , component_2( model[ "entities" ][ 43 ][ "components" ].AddElement() )
            , data( 117 )
        {
            component_1[ "weapons" ];
            MOCK_EXPECT( GetDistance ).with( firer, enemy ).returns( 500 );
            MOCK_EXPECT( CanComponentBeFiredAt ).with( core::Convert( &component_2 ), mock::any ).returns( true );
            component_2[ "volume" ] = volume_1;
            component_2[ "data" ].SetUserData( &data );
        }
        core::Model& component_1;
        core::Model& component_2;
        int data;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_running_and_no_hit_when_firing_at_enemy_and_misses, OneAtOneFiringFixture )
{
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 1 );
    MOCK_EXPECT( ModifyPh ).at_least( 1 ).with( firer, enemy, ammo_1, 0.5 ).returns( 0.9 );
    MOCK_EXPECT( GetPhModificator ).at_least( 1 ).with( firer, enemy, "launcher_1" ).returns( 1 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3u ).returns( 2u );
    MOCK_EXPECT( GetFireRandomNumber ).once().returns( 0 );
    ExpectCallback( 4 );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity/data", "data" )
                              ( "enemy/data", "data" )
                              ( "report", true )
                              ( "paused", false ) );
    ExpectEvent( "direct fire pion",
        sword::test::MakeModel( "component/data", sword::test::MakeUserData( &data ) )
                              ( "dotation", ammo_1 )
                              ( "enemy/data", "data" )
                              ( "entity/identifier", 42 )
                              ( "entity/data", "data" )
                              ( "running", true )
                              ( "use-ph", true )
                              ( "missed", true ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_running_and_hit_when_firing_at_enemy, OneAtOneFiringFixture )
{
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 1 );
    MOCK_EXPECT( ModifyPh ).at_least( 1 ).with( firer, enemy, ammo_1, 0.5 ).returns( 0.9 );
    MOCK_EXPECT( GetPhModificator ).at_least( 1 ).with( firer, enemy, "launcher_1" ).returns( 1 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3u ).returns( 2u );
    MOCK_EXPECT( GetFireRandomNumber ).once().returns( 1 );
    ExpectCallback( 4 );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity/data", "data" )
                              ( "enemy/data", "data" )
                              ( "report", true )
                              ( "paused", false ) );
    ExpectEvent( "direct fire pion",
        sword::test::MakeModel( "component/data", sword::test::MakeUserData( &data ) )
                              ( "dotation", ammo_1 )
                              ( "enemy/data", "data" )
                              ( "entity/identifier", 42 )
                              ( "entity/data", "data" )
                              ( "running", true )
                              ( "use-ph", true )
                              ( "missed", false ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_running_and_no_hit_when_weapon_not_ready, OneAtOneFiringFixture )
{
    MOCK_EXPECT( ModifyDangerosity ).returns( 1 );
    MOCK_EXPECT( ModifyPh ).returns( 1 );
    MOCK_EXPECT( GetPhModificator ).returns( 1 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3u ).returns( 2u );
    MOCK_EXPECT( GetFireRandomNumber ).returns( 1 );
    ExpectCallback( 4 );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity/data", "data" )
                              ( "enemy/data", "data" )
                              ( "report", true )
                              ( "paused", false ) );
    ExpectEvent( "direct fire pion",
        sword::test::MakeModel( "component/data", sword::test::MakeUserData( &data ) )
                              ( "dotation", ammo_1 )
                              ( "enemy/data", "data" )
                              ( "entity/identifier", 42 )
                              ( "entity/data", "data" )
                              ( "running", true )
                              ( "use-ph", true )
                              ( "missed", false ) );
    ExpectCallback( 4 );
    const std::size_t command = StartCommand( "direct fire",
                                              core::MakeModel( "action", 117 )
                                                              ( "identifier", 42 )
                                                              ( "enemy", 51 )
                                                              ( "percentage", 0.07 )
                                                              ( "mode", 0 )
                                                              ( "type", 0 )
                                                              ( "major", false )
                                                              ( "dotation", 0 ) );
    ExpectCallback( 4 );
    ExecuteCommands();
    ExpectCallback( 5 );
    ExpectEvent( "direct fire pion", sword::test::MakeModel( "entity/identifier", 42 )( "running", false ) );
    StopCommand( command );
}

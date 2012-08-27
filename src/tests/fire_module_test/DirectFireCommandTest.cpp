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
            ExpectEvent( "direct fire pion callback", sword::test::MakeModel( "entity", 42 )( "id", mock::any )( "code", 4 ) );
            commands.Start( "direct fire command",
                core::MakeModel( "identifier", 42 )
                    ( "enemy", 51 )
                    ( "percentage", 7 )
                    ( "mode", 0 )
                    ( "dotation", 0 ) );
            mock::verify();
        }
        ~FireFixture()
        {
            ExpectCallback( 5 );
            ExpectEvent( "direct fire pion", sword::test::MakeModel( "entity", 42 )( "running", false ) );
        }
        void ExpectCallback( int code ) // $$$$ MCO 2012-04-27: use RoleAction_DirectFiring::E_ReturnCode ?
        {
            ExpectEvent( "direct fire pion callback", sword::test::MakeModel( "entity", 42 )( "id", mock::any )( "code", code ) );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_impossible_if_target_knowledge_is_invalid, FireFixture )
{
    target[ "valid" ] = false;
    ExpectCallback( 0 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_enemy_destroyed_if_target_is_dead, FireFixture )
{
    target[ "dead" ] = true;
    ExpectCallback( 1 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_temporary_blocked_when_blocked_with_default_coefficient, FireFixture )
{
    MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( true );
    MOCK_EXPECT( GetFireRandomInteger ).returns( 100u );
    ExpectCallback( 6 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_temporary_blocked_when_blocked_with_custom_coefficient, FireFixture )
{
    InitializeDecisional( "<decisional>"
                          "  <urban-combat hit-factor='42'/>"
                          "  <force-ratio default-feedback-time='600s'/>"
                          "</decisional>", 10 );
    MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( true );
    MOCK_EXPECT( GetFireRandomInteger ).returns( 42u );
    ExpectCallback( 6 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_target_has_no_component_when_not_blockable, FireFixture )
{
    InitializeDecisional( "<decisional>"
                          "  <urban-combat hit-factor='42'/>"
                          "  <force-ratio default-feedback-time='600s'/>"
                          "</decisional>", 10 );
    MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( true );
    MOCK_EXPECT( GetFireRandomInteger ).returns( 41u );
    ExpectCallback( 2 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_target_has_no_component, FireFixture )
{
    MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( false );
    ExpectCallback( 2 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_firer_has_no_weapon, FireFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ];
    MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( false );
    ExpectCallback( 2 );
    commands.Execute();
}

namespace
{
    struct WeaponFixture : FireFixture
    {
        WeaponFixture()
            : component_2( entity[ "components" ].AddElement() )
            , weapon   ( component_2[ "weapons" ].AddElement() )
        {
            component_2[ "volume" ] = volume_1;
            component_2[ "component_2" ].SetUserData( &component_2 );
            weapon[ "type" ] = "launcher_1/ammo_1";
            weapon[ "fired-ammo" ] = 0;
            weapon[ "next-time" ] = 0;
            MOCK_EXPECT( IsTemporarilyBlockable ).once().with( firer ).returns( false );
        }
        core::Model& component_2;
        core::Model& weapon;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_capacity_if_firer_cannot_fire, WeaponFixture )
{
    MOCK_EXPECT( CanFire ).with( firer, mock::any, mock::any, 0, 0 ).returns( false );
    ExpectCallback( 2 );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_no_ammunition_when_out_of_dotation, WeaponFixture )
{
    MOCK_EXPECT( CanFire ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    ExpectCallback( 3 );
    commands.Execute();
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
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity", 42 )
            ( "enemy", 43 )
            ( "report", true )
            ( "paused", false ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_enemy_destroyed_when_no_component_can_be_found_to_fire_at, FiringFixture )
{
    core::Model& component_2 = model[ "entities" ][ 43 ][ "components" ].AddElement();
    MOCK_EXPECT( CanComponentBeFiredAt ).once().with( core::Convert( &component_2 ) ).returns( false );
    ExpectCallback( 1 );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity", 42 )
            ( "enemy", 43 )
            ( "report", true )
            ( "paused", false ) );
    commands.Execute();
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
            MOCK_EXPECT( GetDistance ).at_least( 1 ).with( firer, enemy ).returns( 500 );
            MOCK_EXPECT( CanComponentBeFiredAt ).once().with( core::Convert( &component_2 ) ).returns( true );
            component_2[ "volume" ] = volume_1;
            component_2[ "component" ].SetUserData( &data );
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
    MOCK_EXPECT( GetPhModificator ).at_least( 1 ).with( firer, enemy, std::string( "launcher_1" ) ).returns( 1 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3 ).returns( 2 );
    MOCK_EXPECT( GetFireRandomNumber ).once().returns( 0 );
    ExpectCallback( 4 );
    ExpectEffect( weapon, sword::test::MakeModel( "fired-ammo", 2 ) );
    ExpectEffect( weapon, sword::test::MakeModel( "next-time", 0.7 ) );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity", 42 )
            ( "enemy", 43 )
            ( "report", true )
            ( "paused", false ) );
    ExpectEvent( "direct fire pion",
        sword::test::MakeModel( "component", sword::test::MakeUserData( &data ) )
            ( "dotation", ammo_1 )
            ( "enemy", 43 )
            ( "entity", 42 )
            ( "running", true )
            ( "use-ph", true )
            ( "missed", true ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( direct_fire_command_reports_running_and_hit_when_firing_at_enemy, OneAtOneFiringFixture )
{
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 1 );
    MOCK_EXPECT( ModifyPh ).at_least( 1 ).with( firer, enemy, ammo_1, 0.5 ).returns( 0.9 );
    MOCK_EXPECT( GetPhModificator ).at_least( 1 ).with( firer, enemy, std::string( "launcher_1" ) ).returns( 1 );
    MOCK_EXPECT( ReserveAmmunition ).once().with( firer, ammo_1, 3 ).returns( 2 );
    MOCK_EXPECT( GetFireRandomNumber ).once().returns( 1 );
    ExpectCallback( 4 );
    ExpectEffect( weapon, sword::test::MakeModel( "fired-ammo", 2 ) );
    ExpectEffect( weapon, sword::test::MakeModel( "next-time", 0.7 ) );
    ExpectEvent( "direct fire pion attack",
        sword::test::MakeModel( "entity", 42 )
            ( "enemy", 43 )
            ( "report", true )
            ( "paused", false ) );
    ExpectEvent( "direct fire pion",
        sword::test::MakeModel( "component", sword::test::MakeUserData( &data ) )
            ( "dotation", ammo_1 )
            ( "enemy", 43 )
            ( "entity", 42 )
            ( "running", true )
            ( "use-ph", true )
            ( "missed", false ) );
    commands.Execute();
}

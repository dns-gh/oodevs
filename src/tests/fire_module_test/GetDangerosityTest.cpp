// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"
#include "QueryFixture.h"

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_firer_has_no_component, sword::fire::TargetFixture )
{
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_component_is_filtered_out, sword::fire::TargetFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_component_has_no_weapons, sword::fire::TargetFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ];
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_weapon_type_does_not_exist, sword::fire::TargetFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = non_existing_system;
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, "Exception in GetDangerosity hook: Unknown weapon type : " + boost::lexical_cast< std::string >( non_existing_system ) );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_enemy_has_no_component, sword::fire::TargetFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    target[ "major" ] = -1;
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_firer_has_no_ammunition, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_volume_is_invalid, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    component_2[ "volume" ] = 42;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, "Exception in GetDangerosity hook: Invalid target volume identifier in GetDangerosity : 42" );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_ph_modificator_returns_zero, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( "launcher_1" ).returns( 0 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 1 );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_modify_dangerosity_returns_zero, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( "launcher_1" ).returns( 1 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 0 );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_non_zero, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( "launcher_1" ).returns( 2 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 3 );
    BOOST_CHECK_EQUAL( 0.75, GetDangerosity( firer, enemy, &filter, 500, true ) );
}

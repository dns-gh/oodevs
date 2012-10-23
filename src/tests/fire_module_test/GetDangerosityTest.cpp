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

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_firer_has_no_component, QueryFixture )
{
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_component_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_component_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ];
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_weapon_type_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "non existing weapon";
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Exception in GetDangerosity hook: Unknown weapon type : non existing weapon" ) );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_enemy_has_no_component, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_firer_has_no_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_volume_is_invalid, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = 42;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Exception in GetDangerosity hook: Invalid target volume identifier in GetDangerosity : 42" ) );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_ph_modificator_returns_zero, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = volume_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( std::string( "launcher_1" ) ).returns( 0 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 1 );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_zero_when_modify_dangerosity_returns_zero, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = volume_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( std::string( "launcher_1" ) ).returns( 1 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 0 );
    BOOST_CHECK_EQUAL( 0, GetDangerosity( firer, enemy, filter, 500, true ) );
}

BOOST_FIXTURE_TEST_CASE( dangerosity_is_non_zero, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = volume_1;
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( std::string( "launcher_1" ) ).returns( 2 );
    MOCK_EXPECT( ModifyDangerosity ).once().with( core::Convert( &component_2 ), ammo_1 ).returns( 3 );
    BOOST_CHECK_EQUAL( 0.75, GetDangerosity( firer, enemy, filter, 500, true ) );
}

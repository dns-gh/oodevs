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

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_no_ammunition_specified, QueryFixture )
{
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, 0, true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_firer_has_no_component, QueryFixture )
{
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_component_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_component_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_weapon_type_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "non existing weapon";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Unknown weapon type : non existing weapon" ) );
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_minus_one_when_firer_has_no_specified_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_2 ).returns( false );
    BOOST_CHECK_EQUAL( -1, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_non_minus_one_when_firer_has_specified_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_2 ).returns( true );
    BOOST_CHECK_EQUAL( 5000, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), true ) );
}

BOOST_FIXTURE_TEST_CASE( max_range_to_indirect_fire_is_non_minus_one_when_ammunition_is_not_checked, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( 5000, GetMaxRangeToIndirectFire( firer, filter, ammo_2.c_str(), false ) );
}

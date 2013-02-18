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

namespace
{
    const double max = std::numeric_limits< double >::max();
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_min_when_firer_has_no_component, QueryFixture )
{
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, false ) );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, true ) );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_with_checked_ammunition_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_with_unchecked_ammunition_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_without_checked_ammunition_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_without_unchecked_ammunition_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_with_checked_ammunition_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_with_unchecked_ammunition_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_without_checked_ammunition_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_component_without_unchecked_ammunition_has_no_weapons, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_zero_when_weapon_type_with_checked_ammunition_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = invalid;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR,
        "Exception in GetMinRangeToIndirectFire hook: Unknown weapon type : " + boost::lexical_cast< std::string >( invalid ) );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_zero_when_weapon_type_with_unchecked_ammunition_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = invalid;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR,
        "Exception in GetMinRangeToIndirectFire hook: Unknown weapon type : " + boost::lexical_cast< std::string >( invalid ) );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToIndirectFire( firer, &filter, ammo_2, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_zero_when_weapon_type_without_checked_ammunition_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = invalid;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR,
        "Exception in GetMinRangeToIndirectFire hook: Unknown weapon type : " + boost::lexical_cast< std::string >( invalid ) );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToIndirectFire( firer, &filter, 0, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_zero_when_weapon_type_without_unchecked_ammunition_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = invalid;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR,
        "Exception in GetMinRangeToIndirectFire hook: Unknown weapon type : " + boost::lexical_cast< std::string >( invalid ) );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToIndirectFire( firer, &filter, 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_limit_max_when_firer_has_no_specified_checked_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_2;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_2 ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_positive_when_firer_has_no_specified_checked_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_2;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( 100, GetMinRangeToIndirectFire( firer, &filter, 0, true ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_positive_when_firer_has_no_specified_unchecked_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_2;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( 100, GetMinRangeToIndirectFire( firer, &filter, ammo_2, false ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_indirect_fire_is_positive_when_firer_has_specified_checked_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_2;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_2 ).returns( true );
    BOOST_CHECK_EQUAL( 100, GetMinRangeToIndirectFire( firer, &filter, ammo_2, true ) );
}

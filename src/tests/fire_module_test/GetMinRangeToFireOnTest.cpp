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

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_entity_has_no_component, QueryFixture )
{
    entity[ "components" ];
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_component_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_weapon_does_not_exist, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "non existing weapon";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Unknown weapon type : non existing weapon" ) );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_target_has_no_component, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_firer_has_no_ammunition, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_limit_max_when_volume_is_invalid, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = 42;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Invalid target volume identifier in GetMinDistanceForPH : 42" ) );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_zero_when_ph_modificator_is_zero, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = volume_1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( std::string( "launcher_1" ) ).returns( 0 );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_is_non_zero, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    core::Model& component_2 = target[ "components" ].AddElement();
    component_2[ "score" ] = 2;
    component_2[ "volume" ] = volume_1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator2 ).once().with( std::string( "launcher_1" ) ).returns( 1 );
    BOOST_CHECK_LT( 0, GetMinRangeToFireOn( firer, enemy, filter, 0.5 ) );
}

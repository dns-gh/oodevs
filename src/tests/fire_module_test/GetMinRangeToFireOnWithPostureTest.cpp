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

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_limit_max_when_firer_has_no_component, QueryFixture )
{
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_limit_max_when_component_is_filtered_out, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_limit_max_when_firer_has_no_weapon, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ];
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_limit_max_when_enemy_has_no_component, QueryFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    target[ "major" ] = -1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_zero_when_weapon_does_not_exist, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = non_existing_system;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, "Exception in GetMinRangeToFireOnWithPosture hook: Unknown weapon type : " + boost::lexical_cast< std::string >( non_existing_system ) );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_limit_max_when_firer_has_no_ammunition, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( false );
    BOOST_CHECK_EQUAL( max, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_zero_when_volume_is_invalid, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    component_2[ "volume" ] = 42;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, "Exception in GetMinRangeToFireOnWithPosture hook: Invalid target volume identifier in GetMinDistanceForPH : 42" );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_zero_when_ph_modificator_is_zero, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator ).once().with( firer, enemy, "launcher_1" ).returns( 0 );
    BOOST_CHECK_EQUAL( 0, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

BOOST_FIXTURE_TEST_CASE( min_range_to_fire_on_with_posture_is_non_zero, EnemyFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = system_1;
    MOCK_EXPECT( filter ).once().with( core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( HasDotation ).once().with( firer, ammo_1 ).returns( true );
    MOCK_EXPECT( GetPhModificator ).once().with( firer, enemy, "launcher_1" ).returns( 1 );
    BOOST_CHECK_LT( 0, GetMinRangeToFireOnWithPosture( firer, enemy, filter, 0.5 ) );
}

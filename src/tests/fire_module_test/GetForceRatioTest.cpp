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

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_default_max_ratio_when_no_enemy, TargetFixture )
{
    model[ "enemies" ][ 1242 ];
    model[ "friends" ][ 1242 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    BOOST_CHECK_EQUAL( 5, GetForceRatio( core::Convert( &model ), firer ) );
}

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_min_ratio_when_no_friend, TargetFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    BOOST_CHECK_EQUAL( 0.2, GetForceRatio( core::Convert( &model ), firer ) );
}

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_some_ratio, TargetFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ].AddElement() = 52;
    core::Model& f = model[ "knowledges" ][ 1242 ][ "agents" ][ 52 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    MOCK_EXPECT( EvaluateDangerosity2 ).once().with( core::Convert( &f ), enemy ).returns( 2.1 );
    BOOST_CHECK_EQUAL( 3.5, GetForceRatio( core::Convert( &model ), firer ) );
}

BOOST_FIXTURE_TEST_CASE( get_force_ratio_is_cached_on_same_tick, TargetFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    BOOST_CHECK_EQUAL( 0.2, GetForceRatio( core::Convert( &model ), firer ) );
    MOCK_VERIFY( EvaluateDangerosity );
    MOCK_RESET( EvaluateDangerosity );
    BOOST_CHECK_EQUAL( 0.2, GetForceRatio( core::Convert( &model ), firer ) );
    MOCK_VERIFY( EvaluateDangerosity );
    MOCK_RESET( EvaluateDangerosity );
    Tick();
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    BOOST_CHECK_EQUAL( 0.2, GetForceRatio( core::Convert( &model ), firer ) );
}

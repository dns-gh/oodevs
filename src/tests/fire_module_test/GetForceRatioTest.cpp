// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_default_max_ratio_on_tick_zero, sword::fire::ModuleFixture )
{
    model[ "tick" ] = 0;
    BOOST_CHECK_EQUAL( 5, GetForceRatio( core::Convert( &model ), firer ) );
}

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_default_max_ratio_when_no_enemy, sword::fire::ModuleFixture )
{
    model[ "tick" ] = 1;
    model[ "enemies" ][ 1242 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    BOOST_CHECK_EQUAL( 5, GetForceRatio( core::Convert( &model ), firer ) );
}

BOOST_FIXTURE_TEST_CASE( get_force_ratio_returns_some_ratio, sword::fire::ModuleFixture )
{
    model[ "tick" ] = 1;
    model[ "enemies" ][ 1242 ].AddElement() = 43;
    model[ "friends" ][ 1242 ];
    entity[ "fire/force-ratio/feedback-time" ] = 0.1;
    entity[ "knowledges" ] = 1242;
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    BOOST_CHECK_EQUAL( 0.2, GetForceRatio( core::Convert( &model ), firer ) );
}

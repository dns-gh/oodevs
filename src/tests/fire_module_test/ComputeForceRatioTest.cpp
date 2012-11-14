// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"

namespace
{
    MOCK_FUNCTION( callback, 2, bool( const SWORD_Model*, void* ), callback )
}

BOOST_FIXTURE_TEST_CASE( compute_force_ratio_returns_one_when_no_enemy, sword::fire::ModuleFixture )
{
    model[ "enemies" ][ 1242 ];
    model[ "friends" ][ 1242 ];
    BOOST_CHECK_EQUAL( 1, ComputeForceRatio( core::Convert( &model ), firer, callback, 0 ) );
}

BOOST_FIXTURE_TEST_CASE( compute_force_ratio_returns_one_when_enemy_filtered_out, sword::fire::ModuleFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    entity[ "knowledges" ] = 1242;
    int userData;
    MOCK_EXPECT( callback ).once().with( enemy, &userData ).returns( false );
    BOOST_CHECK_EQUAL( 1, ComputeForceRatio( core::Convert( &model ), firer, callback, &userData ) );
}

BOOST_FIXTURE_TEST_CASE( compute_force_ratio_returns_zero_when_no_friend, sword::fire::ModuleFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    entity[ "knowledges" ] = 1242;
    int userData;
    MOCK_EXPECT( callback ).once().with( enemy, &userData ).returns( true );
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    BOOST_CHECK_EQUAL( 0, ComputeForceRatio( core::Convert( &model ), firer, callback, &userData ) );
}

BOOST_FIXTURE_TEST_CASE( compute_force_ratio_returns_some_ratio, sword::fire::ModuleFixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ].AddElement() = 52;
    core::Model& f = model[ "knowledges" ][ 1242 ][ 52 ];
    entity[ "knowledges" ] = 1242;
    int userData;
    MOCK_EXPECT( callback ).once().with( enemy, &userData ).returns( true );
    MOCK_EXPECT( callback ).once().with( core::Convert( &f ), &userData ).returns( true );
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 1 );
    MOCK_EXPECT( EvaluateDangerosity2 ).once().with( core::Convert( &f ), enemy ).returns( 2.34 );
    BOOST_CHECK_EQUAL( 2.34, ComputeForceRatio( core::Convert( &model ), firer, callback, &userData ) );
}

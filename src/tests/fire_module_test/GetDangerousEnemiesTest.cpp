// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"
#include <boost/bind/apply.hpp>

namespace
{
    MOCK_FUNCTION( callback, 2, void( const SWORD_Model*, void* ), callback )
}

BOOST_FIXTURE_TEST_CASE( get_dangerous_enemies_returns_nothing_on_tick_zero, sword::fire::ModuleFixture )
{
    model[ "tick" ] = 0;
    MOCK_EXPECT( callback ).never();
    GetDangerousEnemies( core::Convert( &model ), firer, callback, 0 );
    MOCK_RESET( callback );
}

namespace
{
    struct Fixture : sword::fire::ModuleFixture
    {
        Fixture()
        {
            model[ "tick" ] = 1;
            entity[ "fire/force-ratio/feedback-time" ] = 0.1;
        }
        ~Fixture()
        {
            MOCK_VERIFY( callback );
            MOCK_RESET( callback );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( get_dangerous_enemies_retrieves_nothing_when_entity_has_no_enemies, Fixture )
{
    model[ "enemies" ][ 1242 ];
    int userData;
    GetDangerousEnemies( core::Convert( &model ), firer, callback, &userData );
}

BOOST_FIXTURE_TEST_CASE( get_dangerous_enemies_returns_enemy, Fixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    int userData;
    MOCK_EXPECT( callback ).once().with( enemy, &userData );
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 7 );
    GetDangerousEnemies( core::Convert( &model ), firer, callback, &userData );
}

BOOST_FIXTURE_TEST_CASE( get_dangerous_enemies_is_cached_on_same_tick, Fixture )
{
    model[ "enemies" ][ 1242 ].AddElement() = 51;
    model[ "friends" ][ 1242 ];
    int userData;
    MOCK_EXPECT( callback ).once().with( enemy, &userData );
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 7 );
    GetDangerousEnemies( core::Convert( &model ), firer, callback, &userData );
    MOCK_VERIFY( callback );
    MOCK_VERIFY( EvaluateDangerosity );
    MOCK_RESET( callback );
    MOCK_RESET( EvaluateDangerosity );
    MOCK_EXPECT( callback ).once().with( enemy, &userData );
    GetDangerousEnemies( core::Convert( &model ), firer, callback, &userData );
    MOCK_VERIFY( callback );
    MOCK_VERIFY( EvaluateDangerosity );
    MOCK_RESET( callback );
    MOCK_RESET( EvaluateDangerosity );
    model[ "tick" ] = 2;
    MOCK_EXPECT( callback ).once().with( enemy, &userData );
    MOCK_EXPECT( EvaluateDangerosity ).once().with( enemy, firer ).returns( 7 );
    GetDangerousEnemies( core::Convert( &model ), firer, callback, &userData );
}

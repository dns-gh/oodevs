// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "tic_plugin/RoadFormation.h"
#include "tic_plugin/Movable_ABC.h"

using namespace plugins::tic;
using namespace geometry;
using namespace mockpp;

namespace
{
    class MockMovable : public Movable_ABC
                      , public mockpp::ChainableMockObject
    {
    public:
        MockMovable()
            : mockpp::ChainableMockObject( "MockMovable", 0 )
            , Move_mocker( "Move", this )
        {}

        virtual void Move( const Point2f& to )
        {
            Move_mocker.forward( to );
        }
        virtual void Stop() {};
        mockpp::ChainableMockMethod< void, Point2f > Move_mocker;
    };

    const unsigned count = 12;

    void Test( const std::vector< Point2f >& path, const Point2f& from, MockMovable& movable )
    {
        RoadFormation formation( path );
        formation.Start( from, Vector2f( 1, 0 ), count );
        for( unsigned i = 0; i < count; ++i )
            formation.Apply( &movable );
        movable.verify();
    }
}

// -----------------------------------------------------------------------------
// Name: ::Test_RoadFormation
// Created: AGE 2008-04-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_RoadFormation )
{
    std::vector< Point2f > path;
    path.push_back( Point2f( 0, 0 ) );
    path.push_back( Point2f( 1000, 0 ) );
    path.push_back( Point2f( 1000, 1000 ) );
    path.push_back( Point2f( 0, 1000 ) );

    {
        MockMovable movable;
        movable.Move_mocker.expects( once() ).with( eq( Point2f( 0, 0 ) ) );
        Test( path, Point2f( 0, 0 ), movable );
    }

    {
        MockMovable movable;
        movable.Move_mocker.expects( once() ).with( eq( Point2f( 20, 0 ) ) );
        movable.Move_mocker.expects( once() ).with( eq( Point2f(  0, 0 ) ) );
        Test( path, Point2f( 20, 0 ), movable );
    }

    {
        MockMovable movable;
        for( unsigned i = 0; i < count; ++i )
            movable.Move_mocker.expects( once() ).with( eq( Point2f( 220 - 20 * i, 0 ) ) );
        Test( path, Point2f( 220, 0 ), movable );
    }
    {
        MockMovable movable;
        for( unsigned i = 0; i < 5; ++i )
            movable.Move_mocker.expects( once() ).with( eq( Point2f( 1000, 100 - 20 * i ) ) );
        movable.Move_mocker.expects( once() ).with( eq( Point2f( 1000, 0 ) ) );
        for( unsigned i = 1; i <= 6; ++i )
            movable.Move_mocker.expects( once() ).with( eq( Point2f( 1000 - 20 * i , 0 ) ) );
        Test( path, Point2f( 1000, 100 ), movable );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "MockMovable.h"
#include "tic/RoadFormation.h"
#include <boost/assign/list_of.hpp>

using namespace tic;

namespace
{
    void Test( const std::vector< geometry::Point2f >& path, const geometry::Point2f& from, MockMovable& movable )
    {
        RoadFormation formation( path );
        formation.Start( from, geometry::Vector2f( 1.f, 0.f ), 12u );
        for( unsigned int i = 0; i < 12u; ++i )
            formation.Apply( &movable );
        mock::verify( movable );
    }
}

// -----------------------------------------------------------------------------
// Name: ::Test_RoadFormation
// Created: AGE 2008-04-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_RoadFormation )
{
    std::vector< geometry::Point2f > path = boost::assign::list_of( geometry::Point2f( 0.f, 0.f ) )
                                                                  ( geometry::Point2f( 1000.f, 0.f ) )
                                                                  ( geometry::Point2f( 1000.f, 1000.f ) )
                                                                  ( geometry::Point2f( 0.f, 1000.f ) );
    {
        MockMovable movable;
        MOCK_EXPECT( movable.Stop );
        MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 0.f, 0.f ) );
        Test( path, geometry::Point2f( 0.f, 0.f ), movable );
    }
    {
        MockMovable movable;
        MOCK_EXPECT( movable.Stop );
        MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 20.f, 0.f ) );
        MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 0.f, 0.f ) );
        Test( path, geometry::Point2f( 20.f, 0.f ), movable );
    }
    {
        MockMovable movable;
        MOCK_EXPECT( movable.Stop );
        for( unsigned int i = 0; i < 12u; ++i )
            MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 220.f - 20.f * i, 0.f ) );
        Test( path, geometry::Point2f( 220, 0 ), movable );
    }
    {
        MockMovable movable;
        MOCK_EXPECT( movable.Stop );
        for( unsigned int i = 0; i < 5; ++i )
            MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 1000.f, 100.f - 20.f * i ) );
        MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 1000.f, 0.f ) );
        for( unsigned int i = 1; i <= 6; ++i )
            MOCK_EXPECT( movable.Move ).once().with( geometry::Point2f( 1000.f - 20.f * i, 0.f ) );
        Test( path, geometry::Point2f( 1000, 100 ), movable );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include <directia/brain/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace
{
    class BrainFixture
    {
    public:
        BrainFixture()
        : brain( BRAIN_INIT )
        {
            brain[ "include" ]( std::string("Integration.lua") );
        }
        void MagnitudeTest( directia::tools::binders::ScriptRef var1, directia::tools::binders::ScriptRef var2, double /*expected*/ )
        {
            BOOST_TODO;
            //@TODO MGD replace test with mock on MT_Vector2D
            //directia::tools::binders::ScriptRef magnitude = *brain.GetScriptFunction( "integration.magnitude" );
            //BOOST_CHECK( magnitude( var1, var2 ) );
            //brain.GetScriptFunction( "check" )( magnitude, expected );
        }
        void NormalizedInversedDistanceTest( directia::tools::binders::ScriptRef /*var1*/, directia::tools::binders::ScriptRef /*var2*/, double /*expected*/ )
        {
            //@TODO MGD replace test with mock on MT_Vector2D
            //directia::tools::binders::ScriptRef normalizedInversedDistance = *brain.GetScriptFunction( "integration.normalizedInversedDistance" );
            //BOOST_CHECK( normalizedInversedDistance( var1, var2 ) );
            //brain.GetScriptFunction( "check" )( normalizedInversedDistance, expected );
        }
        directia::tools::binders::ScriptRef CreateVarWithPosition( double x, double y , double z )
        {
            directia::tools::binders::ScriptRef var( brain );
            directia::tools::binders::ScriptRef pos( brain );
            var[ "sim_pos" ] = pos;
            pos[ "x" ] = x;
            pos[ "y" ] = y;
            pos[ "z" ] = z;
            return var;
        }
    public:
        directia::brain::Brain brain;
    };
}
// -----------------------------------------------------------------------------
// Name: Magnitude
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Magnitude, BrainFixture )
{
    MagnitudeTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 0, 0, 0 ), 0. );
    MagnitudeTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 50, 0, 0 ), 50. );
}

// -----------------------------------------------------------------------------
// Name: NormalizedInversedDistance
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NormalizedInversedDistance, BrainFixture )
{
    NormalizedInversedDistanceTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 0, 0, 0 ), 100. );
    NormalizedInversedDistanceTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 10000, 0, 0 ), 0. );
    NormalizedInversedDistanceTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 5000, 0, 0 ), 50.050050050050054 );
}

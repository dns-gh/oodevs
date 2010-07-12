// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include "Fixture.h"

namespace
{
    class BrainFixture : public Fixture
    {
    public:
        void MagnitudeTest( directia::ScriptRef /*var1*/, directia::ScriptRef /*var2*/, double /*expected*/ )
        {
            BOOST_TODO;
            //@TODO MGD replace test with mock on MT_Vector2D
            //directia::ScriptRef magnitude = *brain.GetScriptFunction( "integration.magnitude" );
            //BOOST_CHECK( magnitude( var1, var2 ) );
            //brain.GetScriptFunction( "check" )( magnitude, expected );
        }
        void NormalizedInversedDistanceTest( directia::ScriptRef /*var1*/, directia::ScriptRef /*var2*/, double /*expected*/ )
        {
            BOOST_TODO;
            //@TODO MGD replace test with mock on MT_Vector2D
            //directia::ScriptRef normalizedInversedDistance = *brain.GetScriptFunction( "integration.normalizedInversedDistance" );
            //BOOST_CHECK( normalizedInversedDistance( var1, var2 ) );
            //brain.GetScriptFunction( "check" )( normalizedInversedDistance, expected );
        }
        directia::ScriptRef CreateVarWithPosition( double x, double y , double z )
        {
            directia::ScriptRef var = brain.RegisterObject();
            directia::ScriptRef pos = brain.RegisterObject();
            var.RegisterObject( "sim_pos", pos );
            pos.RegisterObject( "x", x );
            pos.RegisterObject( "y", y );
            pos.RegisterObject( "z", z );
            return var;
        }
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"

#include <directia/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace mockpp;

namespace
{
    void CheckClose( double result, double expected )
    {
        BOOST_CHECK_CLOSE( result, expected, std::numeric_limits<float>::epsilon() );
    }
    void Check( double result, double expected )
    {
        BOOST_CHECK_EQUAL( result, expected );
    }
    class BrainFixture
    {
    public:
        BrainFixture()
        : brain( BOOST_RESOLVE( "." ) ) 
        {
            brain.RegisterFunction< boost::function< void( double, double ) > >( "check", boost::bind( &Check, _1, _2 ) );
            brain.RegisterFunction< boost::function< void( double, double ) > >( "checkClose", boost::bind( &CheckClose, _1, _2 ) );
            brain.GetScriptFunction( "include" )( std::string("Integration.lua") );
        }
        void MagnitudeTest( directia::ScriptRef var1, directia::ScriptRef var2, double expected )
        {
            directia::ScriptRef magnitude = *brain.GetScriptFunction( "integration.magnitude" );
            BOOST_CHECK( magnitude( var1, var2 ) );
            brain.GetScriptFunction( "check" )( magnitude, expected );
        }
        void NormalizedInversedDistanceTest( directia::ScriptRef var1, directia::ScriptRef var2, double expected )
        {
            directia::ScriptRef normalizedInversedDistance = *brain.GetScriptFunction( "integration.normalizedInversedDistance" );
            BOOST_CHECK( normalizedInversedDistance( var1, var2 ) );
            brain.GetScriptFunction( "check" )( normalizedInversedDistance, expected );
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
    public:
        directia::Brain brain;
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
    NormalizedInversedDistanceTest( CreateVarWithPosition( 0, 0, 0 ), CreateVarWithPosition( 5000, 0, 0 ), 50. );
}
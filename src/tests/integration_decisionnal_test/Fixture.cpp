// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include "Fixture.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
    void Empty()
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: Fixture constructor
// Created: SLI 2010-07-12
// -----------------------------------------------------------------------------
Fixture::Fixture()
    : brain( BOOST_RESOLVE( "." ) )
{
    brain.RegisterFunction< boost::function< void() > >( "DEC__StopAction", boost::bind( &Empty ) );
    brain.RegisterFunction< boost::function< void( double, double ) > >( "check", boost::bind( &Check, _1, _2 ) );
    brain.RegisterFunction< boost::function< void( double, double ) > >( "checkClose", boost::bind( &CheckClose, _1, _2 ) );
    brain.GetScriptFunction( "include" )( std::string( "Integration.lua" ) );
    brain.GetScriptFunction( "include" )( std::string( "integration/ToolsFunctions.lua" ) );
}

// -----------------------------------------------------------------------------
// Name: Fixture destructor
// Created: SLI 2010-07-12
// -----------------------------------------------------------------------------
Fixture::~Fixture()
{
    // NOTHING
}

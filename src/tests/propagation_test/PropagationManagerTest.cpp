// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "propagation_test_pch.h"
#include "propagation/PropagationManager.h"
#include <boost/assign.hpp>
#include <boost/filesystem/path.hpp>

namespace
{
    void CheckPath( const std::string& lhs, const std::string& rhs )
    {
        BOOST_CHECK_EQUAL( boost::filesystem::path( lhs ).generic_string(), boost::filesystem::path( rhs ).generic_string() );
    }

    void CheckVector( const std::vector< std::string >& lhs, const std::vector< std::string >& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.size(), rhs.size() );
        for( int i = 0; i < rhs.size(); ++i )
            CheckPath( lhs[ i ], rhs[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: extract_information
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_information )
{
    PropagationManager manager;
    manager.Initialize( BOOST_RESOLVE( "propagation.xml" ) );
    CheckPath( manager.GetProjectionFile(), BOOST_RESOLVE( "projection.prj" ) );

    BOOST_CHECK( manager.GetCurrentFiles( "20100901T121500" ).empty() );

    CheckVector( manager.GetCurrentFiles( "20100901T122100" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetCurrentFiles( "20100901T122900" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetCurrentFiles( "20100901T123000" ), boost::assign::list_of( BOOST_RESOLVE( "fileB" ) ) );
    CheckVector( manager.GetCurrentFiles( "20100901T125100" ), boost::assign::list_of( BOOST_RESOLVE( "fileC" ) )( BOOST_RESOLVE( "fileD" ) ) );
}

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

namespace
{
    void CheckVector( const tools::Path::T_Paths& lhs, const tools::Path::T_Paths& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.size(), rhs.size() );
        for( std::size_t i = 0; i < rhs.size(); ++i )
            BOOST_CHECK_EQUAL( lhs[ i ], rhs[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: extract_information
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_information )
{
    PropagationManager manager( BOOST_RESOLVE( "propagation.xml" ), "" );
    BOOST_CHECK_EQUAL( manager.GetProjectionFile(), BOOST_RESOLVE( "projection.prj" ) );

    BOOST_CHECK( manager.GetFiles( "20100901T121500" ).empty() );

    CheckVector( manager.GetFiles( "20100901T122100" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetFiles( "20100901T122900" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetFiles( "20100901T123000" ), boost::assign::list_of( BOOST_RESOLVE( "fileB" ) ) );
    CheckVector( manager.GetFiles( "20100901T125100" ), boost::assign::list_of( BOOST_RESOLVE( "fileC" ) )( BOOST_RESOLVE( "fileD" ) ) );
}

namespace
{
    unsigned int Convert( const boost::posix_time::ptime& time ) 
    {
        boost::posix_time::ptime epoch( boost::gregorian::date( 1970, 1, 1 ) );
        return ( time - epoch ).total_seconds();
    }
}

// -----------------------------------------------------------------------------
// Name: extract_information_with_a_different_start_time
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_information_with_a_different_start_time )
{
    PropagationManager manager( BOOST_RESOLVE( "propagation.xml" ), "20100901T141011" );
    BOOST_CHECK_EQUAL( manager.GetProjectionFile(), BOOST_RESOLVE( "projection.prj" ) );

    BOOST_CHECK( manager.GetFiles( "20100901T141010" ).empty() );

    CheckVector( manager.GetFiles( "20100901T141011" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetFiles( "20100901T141811" ), boost::assign::list_of( BOOST_RESOLVE( "fileA" ) ) );
    CheckVector( manager.GetFiles( "20100901T141911" ), boost::assign::list_of( BOOST_RESOLVE( "fileB" ) ) );
    CheckVector( manager.GetFiles( "20100901T143911" ), boost::assign::list_of( BOOST_RESOLVE( "fileC" ) )( BOOST_RESOLVE( "fileD" ) ) );
}

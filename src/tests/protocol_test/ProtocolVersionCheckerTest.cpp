// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolVersionChecker.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker_current_version_is_compatible_with_itself
// Created: MCO 2010-12-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtocolVersionChecker_current_version_is_compatible_with_itself )
{
    const sword::ProtocolVersion msg;
    const ProtocolVersionChecker checker( msg );
    BOOST_CHECK( checker.CheckCompatibility() );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : values( Extract( ProtocolVersionChecker::GetCurrentProtocolVersion() ) )
        {}
        bool IsCompatible()
        {
            sword::ProtocolVersion msg;
            msg.set_value( Build( values ) );
            ProtocolVersionChecker checker( msg );
            return checker.CheckCompatibility();
        }
        std::vector< int > values;
    private:
        std::vector< int > Extract( const std::string& v )
        {
            std::vector< std::string > s;
            boost::algorithm::split( s, v, boost::algorithm::is_any_of( "." ), boost::algorithm::token_compress_on );
            std::vector< int > result;
            std::transform( s.begin(), s.end(), std::back_inserter( result ), &boost::lexical_cast< int, std::string > );
            BOOST_REQUIRE_EQUAL( 3u, result.size() );
            return result;
        }
        std::string Build( const std::vector< int >& v )
        {
            std::string result;
            for( std::vector< int >::const_iterator it = v.begin(); it != v.end(); ++it )
            {
                result += boost::lexical_cast< std::string >( *it );
                if( it + 1 != v.end() )
                    result += '.';
            }
            return result;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker_versions_are_compatible_if_major_and_minor_match
// Created: MCO 2010-12-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ProtocolVersionChecker_versions_are_compatible_if_major_and_minor_match, Fixture )
{
    ++values[2];
    BOOST_CHECK( IsCompatible() );
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker_versions_are_incompatible_if_majors_dont_match
// Created: MCO 2010-12-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ProtocolVersionChecker_versions_are_incompatible_if_majors_dont_match, Fixture )
{
    ++values[2];
    BOOST_CHECK( ! IsCompatible() );
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker_versions_are_incompatible_if_minors_dont_match
// Created: MCO 2010-12-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ProtocolVersionChecker_versions_are_incompatible_if_minors_dont_match, Fixture )
{
    ++values[1];
    BOOST_CHECK( ! IsCompatible() );
}

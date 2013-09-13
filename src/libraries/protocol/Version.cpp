// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Version.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#pragma warning( pop )

namespace
{
    std::vector< std::string > Split( const std::string& value )
    {
        std::vector< std::string > result;
        boost::algorithm::split( result, value, boost::algorithm::is_any_of( "." ), boost::algorithm::token_compress_on );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: sword::CheckCompatibility
// Created: MCO 2011-03-24
// -----------------------------------------------------------------------------
bool sword::CheckCompatibility( const std::string& version1, const std::string& version2 )
{
    const std::vector< std::string > actual = Split( version1 );
    const std::vector< std::string > expected = Split( version2 );
    return actual.size() > 0 && expected.size() > 0 &&
        actual[ 0 ] == expected[ 0 ];
}

// -----------------------------------------------------------------------------
// Name: sword::CheckCompatibility
// Created: MCO 2011-01-31
// -----------------------------------------------------------------------------
bool sword::CheckCompatibility( const sword::ProtocolVersion& version )
{
    return CheckCompatibility( version.value(), sword::ProtocolVersion().value() );
}

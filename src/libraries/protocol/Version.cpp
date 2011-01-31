// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include "proto/version.pb.cc"
#pragma warning( pop )

#include "Version.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#pragma warning( pop )

namespace
{
    std::vector< std::string > Split( const sword::ProtocolVersion& version )
    {
        std::string value = version.value();
        std::vector< std::string > result;
        boost::algorithm::split( result, value, boost::algorithm::is_any_of( "." ), boost::algorithm::token_compress_on );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: sword::CheckCompatibility
// Created: MCO 2011-01-31
// -----------------------------------------------------------------------------
bool sword::CheckCompatibility( const sword::ProtocolVersion& version )
{
    const std::vector< std::string > actual = Split( version );
    const std::vector< std::string > expected = Split( sword::ProtocolVersion() );
    return actual.size() > 1 && expected.size() > 1 &&
        actual[ 0 ] == expected[ 0 ] && actual[ 1 ] == expected[ 1 ];
}

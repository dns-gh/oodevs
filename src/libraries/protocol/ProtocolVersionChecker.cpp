// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ProtocolVersionChecker.h"
#include "Protocol.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker constructor
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
ProtocolVersionChecker::ProtocolVersionChecker( const sword::ProtocolVersion& version )
    : protocolVersion_( version.value() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker destructor
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
ProtocolVersionChecker::~ProtocolVersionChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker::CheckCompatibility
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
bool ProtocolVersionChecker::CheckCompatibility() const
{
    std::vector< std::string > actual, expected;
    boost::algorithm::split( actual, protocolVersion_, boost::algorithm::is_any_of( "." ), boost::algorithm::token_compress_on );
    std::string version = GetCurrentProtocolVersion();
    boost::algorithm::split( expected, version, boost::algorithm::is_any_of( "." ), boost::algorithm::token_compress_on );
    return actual.size() > 1 &&
        expected.size() > 1 &&
        actual[ 0 ] == expected[ 0 ] &&
        actual[ 1 ] == expected[ 1 ];
}

// -----------------------------------------------------------------------------
// Name: ProtocolVersionChecker::GetCurrentProtocolVersion
// Created: RPD 2010-05-26
// -----------------------------------------------------------------------------
std::string ProtocolVersionChecker::GetCurrentProtocolVersion()
{
    return sword::ProtocolVersion().value();
}

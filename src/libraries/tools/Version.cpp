// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Version.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>

#include "version_defines.h"

namespace
{
    typedef std::vector< std::string > T_Tokens;

    T_Tokens Split( const std::string& value )
    {
        T_Tokens tokens;
        boost::algorithm::split( tokens, value, boost::is_any_of( "." ) );
        return tokens;
    }

    std::string Splice( const T_Tokens& src, int begin, int end )
    {
        const T_Tokens slice( src.begin() + begin, src.begin() + end );
        return boost::algorithm::join( slice, "." );
    }

    const std::string g_version = APP_VERSION;
    const std::string g_model   = APP_MODEL;
    const T_Tokens    g_tokens  = Split( g_version );
    const std::string g_project = Splice( g_tokens, 0, 2 );
    const std::string g_major   = Splice( g_tokens, 0, 3 );
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return g_version.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppMajorVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppMajorVersion()
{
    return g_major.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppModelVersion()
{
    return g_model.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppProjectVersion
// Created: JSR 2012-01-30
// -----------------------------------------------------------------------------
const char* tools::AppProjectVersion()
{
    return g_project.c_str();
}

// -----------------------------------------------------------------------------
// Name: Version::SplitVersion
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
const std::vector< int > tools::SplitVersion( const std::string& version )
{
    std::vector< int > result;
    std::transform(
        boost::sregex_token_iterator( version.begin(), version.end(), boost::regex( "(\\d+)" ) ),
        boost::sregex_token_iterator(),
        std::back_inserter( result ),
        &boost::lexical_cast< int, std::string > );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Version::CheckVersion
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
bool tools::CheckVersion( const std::string& expectedVersion, const std::string& actualVersion )
{
    const std::vector< int > expected = SplitVersion( expectedVersion );
    const std::vector< int > actual = SplitVersion( actualVersion );
    return expected.empty() || actual.empty() ||
        !std::lexicographical_compare( actual.begin(), actual.end(),
                                       expected.begin(), expected.end() );
}

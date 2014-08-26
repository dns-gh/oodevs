// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "VersionHelper.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <tools/Version.h>
#include <windows.h>
#pragma comment(lib, "version.lib" )

namespace
{
    typedef std::vector< std::string > T_Tokens;

    T_Tokens Split( const std::string& value )
    {
        T_Tokens tokens;
        boost::algorithm::split( tokens, value, boost::is_any_of( "." ) );
        return tokens;
    }

    std::string Splice( const T_Tokens& src, size_t end )
    {
        const T_Tokens slice( src.begin(), src.begin() + std::min( end, src.size() ) );
        return boost::algorithm::join( slice, "." );
    }

    const std::string version = ::tools::GetAppVersion( "5.4.4.0" );
    const std::string model   = "4.8.2";
    const T_Tokens    tokens  = Split( version );
    const std::string project = Splice( tokens, 2 );
    const std::string major   = Splice( tokens, 3 );
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return ::version.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppMajorVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppMajorVersion()
{
    return ::major.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppModelVersion()
{
    return ::model.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppProjectVersion
// Created: JSR 2012-01-30
// -----------------------------------------------------------------------------
const char* tools::AppProjectVersion()
{
    return ::project.c_str();
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
bool tools::CheckVersion( const std::string& expectedVersion, const std::string& actualVersion, unsigned int versionNumber )
{
    std::vector< int > expected = SplitVersion( expectedVersion );
    std::vector< int > actual = SplitVersion( actualVersion );

    if( expected.empty() || actual.empty() )
        return true;

    if( expected.size() > actual.size() )
        actual.resize( expected.size() );
    else
        expected.resize( actual.size() );

    if( versionNumber != 0u )
    {
        actual.resize( versionNumber );
        expected.resize( versionNumber );
    }

    return !std::lexicographical_compare( actual.begin(), actual.end(),
                                       expected.begin(), expected.end() );
}

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
#include <boost/preprocessor/stringize.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/regex.hpp>
#include <algorithm>
#include <vector>

#ifndef APP_MAJOR_VERSION
#   define APP_MAJOR_VERSION 5.2.0
#endif

#ifndef APP_PROJECT_VERSION
#   define APP_PROJECT_VERSION trunk
#endif

#ifndef APP_VERSION
#   define APP_VERSION APP_MAJOR_VERSION##.dev
#endif

//$$$ Crap for Thales
#ifndef APP_MODEL_VERSION
#   define APP_MODEL_VERSION 4.8.2
#endif

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return BOOST_PP_STRINGIZE( APP_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppMajorVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppMajorVersion()
{
    return BOOST_PP_STRINGIZE( APP_MAJOR_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppModelVersion()
{
    return BOOST_PP_STRINGIZE( APP_MODEL_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppProjectVersion
// Created: JSR 2012-01-30
// -----------------------------------------------------------------------------
const char* tools::AppProjectVersion()
{
    return BOOST_PP_STRINGIZE( APP_PROJECT_VERSION );
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

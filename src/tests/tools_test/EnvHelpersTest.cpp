// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/EnvHelpers.h"
#include <tools/TemporaryDirectory.h>

BOOST_AUTO_TEST_CASE( EnvHelpers_TestExpansion )
{
    tools::TemporaryDirectory dir( "envhelpers-", temp_directory );

    std::wstring empty = L"";
    auto parts = tools::ExpandEnvPath( empty, false );
    BOOST_CHECK_EQUAL( 0u, parts.size() );

    // No replacement
    std::wstring env = L"c:\\path1\\path2;%foobar%;c:\\path3";
    parts = tools::ExpandEnvPath( env, false );
    BOOST_REQUIRE_EQUAL( 3u, parts.size() );
    BOOST_CHECK_EQUAL( L"c:\\path1\\path2", parts[0] );
    BOOST_CHECK_EQUAL( L"%foobar%", parts[1] );
    BOOST_CHECK_EQUAL( L"c:\\path3", parts[2] );

    // Replaced
    _putenv( (std::string( "foobar=" ) + dir.Path().ToUTF8()).c_str() );
    parts = tools::ExpandEnvPath( env, false );
    BOOST_REQUIRE_EQUAL( 3u, parts.size() );
    BOOST_CHECK_EQUAL( L"c:\\path1\\path2", parts[0] );
    BOOST_CHECK( dir.Path() == parts[1] );
    BOOST_CHECK_EQUAL( L"c:\\path3", parts[2] );

    // Directory filtering
    parts = tools::ExpandEnvPath( env, true );
    BOOST_REQUIRE_EQUAL( 1u, parts.size() );
    BOOST_CHECK( dir.Path() == parts[0] );
}


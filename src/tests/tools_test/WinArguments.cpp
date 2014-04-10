// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/WinArguments.h"

BOOST_AUTO_TEST_CASE( win_arguments )
{
    BOOST_CHECK( tools::WinArguments( L"app.exe --foo" ).HasOption( "--foo" ) );
    BOOST_CHECK( tools::WinArguments( L"app.exe --foo=true" ).HasOption( "--foo" ) );
    BOOST_CHECK_EQUAL( "true", tools::WinArguments( L"app.exe --foo=true" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK( !tools::WinArguments( L"app.exe --foobar" ).HasOption( "--foo" ) );
    BOOST_CHECK_EQUAL( "true", tools::WinArguments( L"app.exe --foo true" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "true", tools::WinArguments( L"app.exe --foo=true" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "ERROR", tools::WinArguments( L"app.exe --foobar=true" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "ERROR", tools::WinArguments( L"app.exe --foobar true" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "ERROR", tools::WinArguments( L"app.exe --foo --bar" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "ERROR", tools::WinArguments( L"app.exe --foo" ).GetOption( "--foo", "ERROR" ) );
    BOOST_CHECK_EQUAL( "", tools::WinArguments( L"app.exe --foo=" ).GetOption( "--foo", "ERROR" ) );
}

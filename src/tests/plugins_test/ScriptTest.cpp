// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "script_plugin/CommandString.h"
#include <directia/Brain.h>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;
using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ScriptTest_CommandParsing
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ScriptTest_CommandParsing )
{
    {
        CommandString command( "/mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
    }
    {
        CommandString command( "/mycommand arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
    }
    {
        CommandString command( "/mycommand arg1 arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
    }
    {
        CommandString command( "/mycommand 'my arg1'" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
    }
    {
        CommandString command( "/mycommand \"my arg1\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
    }
    {
        CommandString command( "/mycommand 'my arg1' arg2 \"my arg3\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "my arg3" );
    }
    {
        CommandString command( "/mycommand arg1 arg2 arg3" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "arg3" );
    }
    {
        CommandString command( "/mycommand 'my \"arg1' arg2 \"my 'arg3\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my \"arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "my 'arg3" );
    }
    {
        CommandString command( "/mycommand \"my \\\"arg1\\\"\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
//        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my \"arg1\"" );
    }
}

namespace
{
    void TestLibrary( const std::string& name )
    {
        const std::string filename = BOOST_RESOLVE( "script_plugin/" + name + ".lua" );
        directia::Brain( bfs::path( filename, bfs::native ).native_directory_string(), "test_suite" ).GetScriptFunction( "RunTest" )();
    }
}

// -----------------------------------------------------------------------------
// Name: ScriptTest_ActionsLibTest
// Created: SBO 2009-08-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ScriptTest_ActionsLibTest )
{
    TestLibrary( "actions" );
}

// -----------------------------------------------------------------------------
// Name: ScriptTest_ArtilleryLibTest
// Created: SBO 2009-08-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ScriptTest_ArtilleryLibTest )
{
    TestLibrary( "artillery" );
}

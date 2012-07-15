// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "script_plugin/CommandString.h"
#include <directia/brain/Brain.h>
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

#include <iostream>

namespace
{
    void TestLibrary( const std::string& name )
    {
        std::string filename = BOOST_RESOLVE( "script_plugin/" + name + ".lua" );
        std::size_t lookHere = 0;
        std::size_t foundHere;
        while( ( foundHere = filename.find( "\\", lookHere ) ) != std::string::npos )
        {
            filename.replace( foundHere, 1, "/" );
            lookHere = foundHere + 1;
        }
        const std::string brainParam = "brain={file='" + filename + "',type='test_suite'} plugins={} cwd='" + bfs::path( filename ).branch_path().string() + "'";
        std::cerr << "Param = " << brainParam << std::endl;
        directia::brain::Brain brain( brainParam );
        brain[ "RunTest" ]();
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

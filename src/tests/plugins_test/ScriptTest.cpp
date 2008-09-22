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

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ScriptTest_CommandParsing )
{
    {
        script::CommandString command( "/mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
    }
    {
        script::CommandString command( "/mycommand arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
    }
    {
        script::CommandString command( "/mycommand arg1 arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
    }
    {
        script::CommandString command( "/mycommand 'my arg1'" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
    }
    {
        script::CommandString command( "/mycommand \"my arg1\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
    }
    {
        script::CommandString command( "/mycommand 'my arg1' arg2 \"my arg3\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "my arg3" );
    }
    {
        script::CommandString command( "/mycommand arg1 arg2 arg3" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "arg3" );
    }
    {
        script::CommandString command( "/mycommand 'my \"arg1' arg2 \"my 'arg3\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my \"arg1" );
        BOOST_CHECK_EQUAL( command.Arg( 2 ), "arg2" );
        BOOST_CHECK_EQUAL( command.Arg( 3 ), "my 'arg3" );
    }
    {
        script::CommandString command( "/mycommand \"my \\\"arg1\\\"\"" );
        BOOST_CHECK_EQUAL( command.Arg( 0 ), "mycommand" );
//        BOOST_CHECK_EQUAL( command.Arg( 1 ), "my \"arg1\"" );
    }
}

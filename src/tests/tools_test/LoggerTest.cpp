// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "MT_Tools/MT_FileLogger.h"
#include "tools/FileWrapper.h"
#include "tools/Main.h"
#include <tools/TemporaryDirectory.h>
#include <boost/assign.hpp>
#include <boost/regex.hpp>

namespace
{
    // This is silly bug log interface does not let us mock the date, yet.
    std::string FixLogLine( const std::string& line )
    {
        boost::regex re( "^\\[[^\\]]+\\](.*)$", boost::regex::perl );
        return boost::regex_replace( line, re, "[DATE]\\1" );
    }

    std::vector< std::string > ParseLog( const tools::Path& path )
    {
        std::vector< std::string > lines;
        tools::Fstream file( path, std::ios::in );
        std::string line;
        while( std::getline( file, line ) )
            lines.push_back( FixLogLine( line ) );
        return lines;
    }

    struct Fixture
    {
        Fixture()
            : temp( "filelogger-", temp_directory )
            , loggerFile( temp.Path() / "log.txt" )
        {}
        tools::TemporaryDirectory temp;
        tools::Path loggerFile;
    };
}

BOOST_FIXTURE_TEST_CASE( filelogger_basics, Fixture )
{
    {
        MT_FileLogger logger( loggerFile, 1, 1000, MT_Logger_ABC::eLogLevel_All );
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message", "context", 42 );
        logger.Pause();
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message_paused", "context", 42 );
        logger.Resume();
        logger.Log( MT_Logger_ABC::eLogLevel_Error, "message_nocode", "context", -1 );
        logger.Log( MT_Logger_ABC::eLogLevel_Debug, "message_nocontext_nocode", 0, -1 );
        logger.Log( MT_Logger_ABC::eLogLevel_FatalError, "message_nocontext", 0, 42 );
        logger.Log( MT_Logger_ABC::eLogLevel_All, 0, "no message", 42 );
    }
    const std::vector< std::string > expected = boost::assign::list_of( "[DATE] <Simulation> <info> message(42) [Context: context]" )
        ( "[DATE] <Simulation> <functERR> message_nocode[Context: context]" )
        ( "[DATE] <Simulation> <debug> message_nocontext_nocode" )
        ( "[DATE] <Simulation> <fatalERR> message_nocontext(42) " )
        ( "[DATE] <Simulation> <Unknown log level> (42) [Context: no message]" );
    const std::vector< std::string > actual = ParseLog( loggerFile );
    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), actual.begin(), actual.end() );
}

BOOST_AUTO_TEST_CASE( getloglevel )
{
    BOOST_CHECK_EQUAL( 0, tools::GetLogLevel( "unknown" ) );

    // "warning" gives warnings and more important messages
    const int level = MT_Logger_ABC::eLogLevel_FatalError
        + MT_Logger_ABC::eLogLevel_Error
        + MT_Logger_ABC::eLogLevel_Warning;
    BOOST_CHECK_EQUAL( level, tools::GetLogLevel( "warning" ) );
}

BOOST_FIXTURE_TEST_CASE( filelogger_does_not_truncate_content_by_default, Fixture )
{
    {
        tools::Ofstream file( loggerFile );
        file << "some pre-existing content" << std::endl;
    }
    {
        MT_FileLogger logger( loggerFile, 1, 1000, MT_Logger_ABC::eLogLevel_All );
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message", "context", 42 );
    }
    const std::vector< std::string > expected = boost::assign::list_of( "some pre-existing content" )
        ( "[DATE] <Simulation> <info> message(42) [Context: context]" );
    const std::vector< std::string > actual = ParseLog( loggerFile );
    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), actual.begin(), actual.end() );
}

BOOST_FIXTURE_TEST_CASE( filelogger_truncates_content_when_configured_to, Fixture )
{
    {
        tools::Ofstream file( loggerFile );
        file << "some pre-existing content" << std::endl;
    }
    {
        MT_FileLogger logger( loggerFile, 1, 1000, MT_Logger_ABC::eLogLevel_All, true );
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message", "context", 42 );
    }
    const std::vector< std::string > expected = boost::assign::list_of( "[DATE] <Simulation> <info> message(42) [Context: context]" );
    const std::vector< std::string > actual = ParseLog( loggerFile );
    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), actual.begin(), actual.end() );
}

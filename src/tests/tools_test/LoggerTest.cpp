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
#include <tools/TemporaryDirectory.h>
#include <boost/regex.hpp>

namespace
{

// This is silly bug log interface does not let us mock the date, yet.
std::string FixLogLine( std::string& line )
{
    boost::regex re( "^\\[[^\\]]+\\](.*)$", boost::regex::perl );
    return boost::regex_replace( line, re, "[DATE]\\1" );
}

void ParseLog( const tools::Path& path, std::vector< std::string >& lines )
{
    lines.clear();
    tools::Fstream file( path, std::ios::in );
    std::string line;
    while( std::getline( file, line ))
    {
        lines.push_back( FixLogLine( line ));
    }
}

void CheckLines( const char** expected, const tools::Path& path )
{
    std::vector< std::string > lines;
    ParseLog( path, lines);

    const char** e = expected;
    while( *e )
        ++e;
    size_t expectedCount = e - expected;
    BOOST_REQUIRE_EQUAL( expectedCount, lines.size() );

    for( size_t i = 0; i != expectedCount; ++i )
    {
        BOOST_CHECK_EQUAL( expected[i], lines[i] );
    }
}

}  //namespace

BOOST_AUTO_TEST_CASE( filelogger_basics )
{
    tools::TemporaryDirectory temp( "filelogger-", temp_directory );
    tools::Path loggerFile = tools::Path::FromUnicode( temp.path().wstring() ) / "log.txt";
    {
        MT_FileLogger logger( loggerFile, 1, 1000, MT_Logger_ABC::eLogLevel_All, false, MT_Logger_ABC::eSimulation, false );
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message", "context", 42 );
        logger.Pause();
        logger.Log( MT_Logger_ABC::eLogLevel_Info, "message_paused", "context", 42 );
        logger.Resume();
        logger.Log( MT_Logger_ABC::eLogLevel_Error, "message_nocode", "context", -1 );
        logger.Log( MT_Logger_ABC::eLogLevel_Debug, "message_nocontext_nocode", 0, -1 );
        logger.Log( MT_Logger_ABC::eLogLevel_FatalError, "message_nocontext", 0, 42 );
        logger.Log( MT_Logger_ABC::eLogLevel_All, 0, "no message", 42 );
    }
    {
        const char* expected[] =
        {
            "[DATE] <Simulation> <info> message(42) [Context: context]",
            "[DATE] <Simulation> <functERR> message_nocode[Context: context]",
            "[DATE] <Simulation> <debug> message_nocontext_nocode",
            "[DATE] <Simulation> <fatalERR> message_nocontext(42) ",
            "[DATE] <Simulation> <Unknown log level> (42) [Context: no message]",
            0,
        };
        CheckLines( expected, loggerFile );
    }
}

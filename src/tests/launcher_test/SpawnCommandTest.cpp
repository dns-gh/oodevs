// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "launcher_test_pch.h"
#include "frontend/SpawnCommand.h"
#include "tools/GeneralConfig.h"
#include <tools/TemporaryDirectory.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace bfs = boost::filesystem;

namespace
{

class DummyCommand: public frontend::SpawnCommand
{
public:
    DummyCommand( const tools::GeneralConfig& config, const tools::Path& exe )
        : frontend::SpawnCommand( config, exe, true )
    {
    }

    void AddArgument( const std::string& s )
    {
        frontend::SpawnCommand::AddArgument( QString( s.c_str() ));
    }
};

} // namespace

BOOST_AUTO_TEST_CASE( TestSpawnCommand )
{
    tools::TemporaryDirectory tempDir( "launcher_test-", temp_directory );

    // Try to run some python command from the packaged python
    bfs::path rootDir = BOOST_RESOLVE( "../../.." ).ToBoost();
    bfs::path python = rootDir / "bin/python/python.exe";
    BOOST_REQUIRE( bfs::exists( python ));

    tools::GeneralConfig config( "c:/foobar" );

    bfs::path output = tempDir.path() / "output.txt";
    std::string arg = output.string();
    boost::replace_all(arg, "\\", "/");
    tools::Path exe = tools::Path::FromUTF8( python.string() );
    // Fill output.txt with python command line arguments after -c
    DummyCommand cmd( config, exe );
    cmd.AddArgument( "-c" );
    cmd.AddArgument( "\"import sys; file('" + arg + "', 'wb').write(" + "' '.join(sys.argv[1:]))\"" );
    cmd.AddArgument( "arg1" );
    cmd.AddArgument( "arg2" );

    cmd.Start();
    bool stopped = !cmd.Wait( boost::posix_time::minutes(1) );
    BOOST_CHECK( stopped );
    std::fstream f( output.string(), std::ios::in );
    std::string line;
    BOOST_CHECK( std::getline( f, line ));
    BOOST_CHECK_EQUAL( "arg1 arg2", line );
}

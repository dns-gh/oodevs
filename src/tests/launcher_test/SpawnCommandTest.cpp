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
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace
{

class DummyCommand: public frontend::SpawnCommand
{
public:
    DummyCommand( const tools::GeneralConfig& config, const tools::Path& exe )
        : frontend::SpawnCommand( config, exe, "dummy" )
    {
        // NOTHING
    }
};

} // namespace

BOOST_AUTO_TEST_CASE( TestSpawnCommand )
{
    tools::TemporaryDirectory tempDir( "launcher_test-", temp_directory );

    // Try to run some python command from the packaged python
    tools::Path rootDir = BOOST_RESOLVE( "../../.." );
    tools::Path python = rootDir / "bin/python/python.exe";
    BOOST_REQUIRE( python.Exists() );

    tools::GeneralConfig config( "c:/foobar" );

    tools::Path output = tempDir.Path() / "output.txt";

    // Fill output.txt with python command line arguments after -c
    DummyCommand cmd( config, python );
    cmd.AddArgument( "-c" );
    cmd.AddArgument( "import sys; file('" + output.Normalize().ToUTF8() + "', 'wb').write(" + "' '.join(sys.argv[1:]))" );
    cmd.AddArgument( "arg1" );
    cmd.AddArgument( "arg2" );

    cmd.Start();
    bool stopped = cmd.Wait( boost::posix_time::minutes(1) );
    BOOST_CHECK( stopped );
    tools::Fstream f( output, std::ios::in );
    std::string line;
    BOOST_CHECK( std::getline( f, line ));
    BOOST_CHECK_EQUAL( "arg1 arg2", line );
}

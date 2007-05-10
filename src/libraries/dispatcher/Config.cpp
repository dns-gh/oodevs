// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Config.h"

#pragma warning( push )
#pragma warning( disable: 4127 4244 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include "xeumeuleu/xml.h"

namespace po = boost::program_options;
namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config()
    : tools::GeneralConfig()
{
    po::options_description desc( "Dispatcher/replayer options" );
//    desc.add_options()
//        ( "nothing"     , "specify checkpoint to load"               )
//        ( "test"        ,                                                  "test mode: loading + first tick"          )
//        ( "testdata"    ,                                                  "test mode: load models only (no terrain)" )
//    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void Config::Parse( int argc, char** argv )
{
    tools::GeneralConfig::Parse( argc, argv );

    xml::xifstream xisExercise( GetExerciseFile() );
    xisExercise >> xml::start( "exercise" )
                    >> xml::start( "profiles" )
                        >> xml::attribute( "file", profiles_ );

    xml::xifstream xisGame( GetGameFile() );
    xisGame >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "network" )
                        >> xml::attribute( "client", networkSimulationParameters_ )
                        >> xml::attribute( "server", networkClientsParameters_    )
                    >> xml::end()
                    >> xml::start( "recorder" )
                        >> xml::attribute( "enabled", bRecorderEnabled_ );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Config::GetProfilesFile
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
std::string Config::GetProfilesFile() const
{
    return BuildExerciseChildFile( profiles_ );
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkSimulationParameters
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
const std::string& Config::GetNetworkSimulationParameters() const
{
    return networkSimulationParameters_;
}
// -----------------------------------------------------------------------------
// Name: Config::GetNetworkClientsParameters
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
unsigned short Config::GetNetworkClientsParameters() const
{
    return networkClientsParameters_;
}

// -----------------------------------------------------------------------------
// Name: Config::RecorderEnabled
// Created: NLD 2007-05-10
// -----------------------------------------------------------------------------
bool Config::RecorderEnabled() const
{
    return bRecorderEnabled_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetRecorderDirectory
// Created: NLD 2007-05-10
// -----------------------------------------------------------------------------
std::string Config::GetRecorderDirectory( const std::string& records ) const
{
    return BuildGameChildFile( ( bfs::path( "records", bfs::native ) / bfs::path( records, bfs::native ) ).native_file_string() );
}

// -----------------------------------------------------------------------------
// Name: Config::GenerateRecorderDirectory
// Created: NLD 2007-05-10
// -----------------------------------------------------------------------------
std::string Config::GenerateRecorderDirectory() const
{
    return GetRecorderDirectory( bpt::to_iso_string( bpt::second_clock::local_time() ) );
}


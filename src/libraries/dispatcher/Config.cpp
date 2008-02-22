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
#pragma warning( pop )
#include "xeumeuleu/xml.h"

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

using namespace dispatcher;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config()
    : tools::GeneralConfig()
    , networkClientsParameters_( 0 )
{
    po::options_description desc( "Dispatcher/replayer options" );
    desc.add_options()
        ( "port" , po::value( &networkClientsParameters_ ), "specify the serving port" )
    ;
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

    std::string dataset, physical;
    xifstream xisExercise( GetExerciseFile() );
    xisExercise >> start( "exercise" )
                    >> start( "profiles" )
                        >> attribute( "file", profiles_ )
                    >> end()
                    >> xml::start( "model" )
                        >> xml::attribute( "dataset", dataset )
                        >> xml::attribute( "physical", physical )
                    >> xml::end()
                >> xml::end();
    physical_ = tools::GeneralConfig::GetPhysicalFile( dataset, physical );

    int port;
    xml::xifstream xisGame( GetSessionFile() );
    xisGame >> start( "session" )
                >> start( "config" )
                    >> start( "dispatcher" )
                        >> start( "network" )
                            >> attribute( "client", networkSimulationParameters_ )
                            >> attribute( "server", port )
                        >> end()
                    >> end()
                >> end()
            >> end();
    if( ! networkClientsParameters_ )
        networkClientsParameters_ = unsigned short( port );
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
// Name: Config::GetPhysicalFile
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
const std::string& Config::GetPhysicalFile() const
{
    return physical_;
}

namespace
{
    std::string BuildChildPath( const std::string& parent, const std::string& child )
    {
        return ( bfs::path( parent, bfs::native ).branch_path() / bfs::path( child, bfs::native ) ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: Config::BuildPhysicalChildFile
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
std::string Config::BuildPhysicalChildFile( const std::string& file ) const
{
    return BuildChildPath( GetPhysicalFile(), file );
}

// -----------------------------------------------------------------------------
// Name: Config::GetRecordDirectory
// Created: AGE 2008-01-07
// -----------------------------------------------------------------------------
std::string Config::GetRecordDirectory() const
{
    return BuildSessionChildFile( "record" );
}

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
#include "PluginConfig.h"

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
    xml::xifstream xisGame( GetGameFile() );
    xisGame >> start( "config" )
                >> start( "dispatcher" )
                    >> start( "network" )
                        >> attribute( "client", networkSimulationParameters_ )
                        >> attribute( "server", port )
                    >> end()
                    >> start( "plugins" )
                        >> list( "plugin", *this, &Config::ReadPlugin );
    networkClientsParameters_ = unsigned short( port );
}

// -----------------------------------------------------------------------------
// Name: Config::ReadPlugin
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void Config::ReadPlugin( xml::xistream& xis )
{
    std::auto_ptr< PluginConfig > plugin( new PluginConfig( xis ) );
    plugins_[plugin->GetName()] = plugin.release();
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
// Name: Config::GetPluginConfig
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
const PluginConfig& Config::GetPluginConfig( const std::string& name ) const
{
    T_Plugins::const_iterator it = plugins_.find( name );
    if( it == plugins_.end() )
        throw std::runtime_error( "Unable to find configuration information for plugin: " + name );
    return *it->second;
}

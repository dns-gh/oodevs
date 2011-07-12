// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PluginFactory.h"
#include "Config.h"
#include "Model.h"
#include "StaticModel.h"
#include "CompositePlugin.h"
#include "SimulationPublisher_ABC.h"
#include "ClientsNetworker.h"
#include "DispatcherPlugin.h"
#include "FileLogger.h"
#include "PluginFactory_ABC.h"
#include "rights_plugin/RightsPlugin.h"
#include "logger_plugin/LoggerPlugin.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "MT_Tools/MT_Logger.h"
#include "saver_plugin/SaverPlugin.h"
#include "script_plugin/ScriptPlugin.h"
#include "score_plugin/ScorePlugin.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>
#include <windows.h>

using namespace dispatcher;
using namespace plugins;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory( const Config& config, Model& model, const dispatcher::StaticModel& staticModel,
                              SimulationPublisher_ABC& simulation, ClientsNetworker& clients, CompositePlugin& handler,
                              CompositeRegistrable& registrables, const Services& services, RotatingLog& log, int maxConnections )
    : config_      ( config )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , simulation_  ( simulation )
    , clients_     ( clients )
    , handler_     ( handler )
    , registrables_( registrables )
    , rights_      ( new plugins::rights::RightsPlugin( model_, clients_, config_, clients_, handler_, clients_, registrables, maxConnections ) )
    , services_    ( services )
{
    handler_.Add( rights_ );
    handler_.Add( new DispatcherPlugin( model_, simulation_, clients_, *rights_, log ) );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory destructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
{
    for( T_Factories::const_iterator it = factories_.begin(); it != factories_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Register
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::Register( PluginFactory_ABC& factory )
{
    factories_.push_back( &factory );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Instanciate
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::Instanciate()
{
    // $$$$ AGE 2008-08-04: retirer la dépendance...
    handler_.Add( new messenger::MessengerPlugin( clients_, clients_, clients_, config_, registrables_ ) );
    handler_.Add( new script::ScriptPlugin( model_, staticModel_, config_, simulation_, clients_, clients_, *rights_, registrables_ ) );
    handler_.Add( new score::ScorePlugin( clients_, clients_, clients_, config_, registrables_ ) );
    handler_.Add( new logger::LoggerPlugin( model_, staticModel_, config_, services_ ) );
    xml::xifstream xis( config_.GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "plugins" )
                        >> xml::list( *this, &PluginFactory::ReadPlugin );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::ReadPlugin
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::ReadPlugin( const std::string& name, xml::xistream& xis )
{
    if( xis.has_attribute( "library" ) )
        LoadPlugin( name, xis );
    else if( name == "recorder" )
        handler_.Add( new plugins::saver::SaverPlugin( model_, config_ ) );
    else
    {
        for( T_Factories::const_iterator it = factories_.begin(); it != factories_.end(); ++it )
        {
            std::auto_ptr< Plugin_ABC > plugin( (*it)->Create( name, xis, config_, model_, staticModel_, simulation_, clients_, clients_ , clients_, registrables_ ) );
            if( plugin.get() )
                handler_.Add( plugin.release() );
        }
    }
}

namespace
{
#ifdef _DEBUG
# define EXTENSION "-gd"
#else
# define EXTENSION ""
#endif

    std::string SetLibraryConfiguration( const std::string& libraryPath )
    {
        boost::filesystem::path p( libraryPath );
        return boost::filesystem::basename( p ) + EXTENSION + boost::filesystem::extension( p );
    }

    typedef dispatcher::Plugin_ABC* (*CreateFunctor)( dispatcher::Model_ABC&, const dispatcher::StaticModel&, dispatcher::SimulationPublisher_ABC&, const dispatcher::Config&, dispatcher::Logger_ABC&, xml::xistream& );
    typedef void (*DestroyFunctor)( dispatcher::Plugin_ABC*, dispatcher::Logger_ABC& );

    template< typename T >
    T LoadFunction( HMODULE& module, const std::string& name )
    {
        T function = (T)GetProcAddress( module, name.c_str() );
        if( !function )
            throw std::runtime_error( "unable to find function '" + name + "'" );
        return function;
    }

    const unsigned int environment_buffer_size = 32767;

    struct EnvironmentGuard
    {
        explicit EnvironmentGuard( const std::string& directory )
        {
            memset( path_, 0, environment_buffer_size );
            if( GetEnvironmentVariable( "Path", path_, environment_buffer_size ) == 0 )
                throw std::runtime_error( "Failed to retrieve 'PATH' environment variable." );
            const std::string newPath( directory + ";" + std::string( path_ ) );
            if( !SetEnvironmentVariable( "Path", newPath.c_str() ) )
                throw std::runtime_error( "Failed to set 'PATH' environment variable." );
        }
        ~EnvironmentGuard()
        {
            if( !std::string( path_ ).empty() )
                SetEnvironmentVariable( "Path", path_ );
        }
        char path_[ environment_buffer_size ];
    };
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::LoadPlugin
// Created: SBO 2011-01-31
// -----------------------------------------------------------------------------
void PluginFactory::LoadPlugin( const std::string& name, xml::xistream& xis )
{
    try
    {
        const EnvironmentGuard environment( config_.BuildPluginDirectory( name ) );
        const std::string library = SetLibraryConfiguration( xis.attribute< std::string >( "library" ) );
        HMODULE module = LoadLibrary( library.c_str() );
        if( !module )
            throw std::runtime_error( "failed to load library: '" + library + "'" );
        CreateFunctor createFunction = LoadFunction< CreateFunctor >( module, "CreateInstance" );
        DestroyFunctor destroyFunction = LoadFunction< DestroyFunctor >( module, "DestroyInstance" );
        boost::shared_ptr< Logger_ABC > logger( new FileLogger( config_.BuildSessionChildFile( name + "_plugin.log" ) ) );
        boost::shared_ptr< Plugin_ABC > plugin( createFunction( model_, staticModel_, simulation_, config_, *logger, xis ), boost::bind( destroyFunction, _1, boost::ref( *logger ) ) );
        if( !plugin.get() )
            throw std::runtime_error( "CreateFunctor returned an error (see details in plugin log file)" );
        handler_.Add( plugin, logger );
        MT_LOG_INFO_MSG( "Plugin '" << name << "' loaded (file: " << library << ")" );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Failed to load plugin '" << name << "' reason: " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Failed to load plugin '" << name << "', unknown reason" );
    }
}

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
#include "PluginContainer.h"
#include "SimulationPublisher_ABC.h"
#include "ClientsNetworker.h"
#include "DispatcherPlugin.h"
#include "FileLogger.h"
#include "PluginFactory_ABC.h"
#include "CheckpointFilterPlugin.h"
#include "rights_plugin/RightsPlugin.h"
#include "logger_plugin/LoggerPlugin.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "MT_Tools/MT_Logger.h"
#include "logistic_plugin/LogisticPlugin.h"
#include "order_plugin/OrderPlugin.h"
#include "saver_plugin/SaverPlugin.h"
#include "score_plugin/ScorePlugin.h"
#include "script_plugin/ScriptPlugin.h"
#include "vision_plugin/VisionPlugin.h"
#include "tools/FileWrapper.h"
#include "tools/XmlStreamOperators.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>
#include <windows.h>

using namespace dispatcher;
using namespace plugins;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory( const Config& config, const boost::shared_ptr< Model >& model,
    const dispatcher::StaticModel& staticModel, SimulationPublisher_ABC& simulation,
    const boost::shared_ptr< ClientsNetworker >& clients, PluginContainer& handler,
    CompositeRegistrable& registrables, const Services& services, tools::Log& log,
    int maxConnections )
    : config_      ( config )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , simulation_  ( simulation )
    , clients_     ( clients )
    , rootHandler_ ( handler )
    , registrables_( registrables )
    , rights_      ( new plugins::rights::RightsPlugin( *model_, *clients_,
        config_, *clients_, rootHandler_, *clients_, registrables, maxConnections, false ) )
    , checkpointFilter_( new CheckpointFilterPlugin( *rights_ ) )
    , pOrder_      ( new plugins::order::OrderPlugin( config_, *model_, simulation_ ) )
    , services_    ( services )
{
    clients_->RegisterMessage( *this, &PluginFactory::Receive );

    // rootHandler_ is the root element of the plugin tree. Parent plugins can
    // prevent specific message to reach their descendants. All other actions
    // are forwarded unconditionally. The registration order is also important:
    // - DispatcherPlugin forwards to clients
    // - Model is used by other plugins and also triggers events on Entity_ABC::Update
    // - SaverPlugin uses the Model
    rootHandler_.Add( checkpointFilter_ );
    checkpointFilter_->Add( rights_ );
    checkpointFilter_->Add( pOrder_ );
    checkpointFilter_->Add( boost::make_shared< DispatcherPlugin >(
                simulation_, *clients_, *rights_, *pOrder_, log ) );

    // Vision plugin prevents vision cones from reaching ClientsNetworker and
    // being broadcast.
    auto vision = boost::make_shared< vision::VisionPlugin >( *model_, *clients_,
            simulation_, *rights_ );
    checkpointFilter_->Add( vision );
    vision->Add( clients_ );

    checkpointFilter_->AddHandler( model_ );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory destructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
{
    // NOTHING
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
    checkpointFilter_->Add( boost::make_shared< messenger::MessengerPlugin >(
                *clients_, *clients_, *clients_, config_, registrables_ ) );
    checkpointFilter_->Add( boost::make_shared< script::ScriptPlugin >(
                *model_, config_, simulation_, *clients_, *clients_, *rights_, registrables_ ) );
    checkpointFilter_->Add( boost::make_shared< score::ScorePlugin >(
                *clients_, *clients_, *clients_, config_, registrables_ ) );
    checkpointFilter_->Add( boost::make_shared< logger::LoggerPlugin >( *model_,
                staticModel_, config_, services_ ) );
    checkpointFilter_->Add( logistic::CreateLogisticPlugin( *model_, staticModel_, config_ ) );
    tools::Xifstream xis( config_.GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "plugins" )
                        >> xml::list( *this, &PluginFactory::ReadPlugin );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Close
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void PluginFactory::Close()
{
    rootHandler_.Close();
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::ReadPlugin
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::ReadPlugin( const std::string& name, xml::xistream& xis )
{
    if( xis.has_attribute( "library" ) )
        LoadPlugin( tools::Path::FromUTF8( name ), xis );
    else if( name == "recorder" )
        checkpointFilter_->Add( boost::make_shared< plugins::saver::SaverPlugin >( *clients_, *model_, config_ ) );
    else
    {
        for( auto it = factories_.begin(); it != factories_.end(); ++it )
        {
            auto plugin = it->Create( name, xis, config_, *model_, staticModel_,
                    simulation_, *clients_, *clients_ , *clients_, registrables_ );
            if( plugin )
                checkpointFilter_->Add( plugin );
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

    tools::Path SetLibraryConfiguration( const tools::Path& libraryPath )
    {
        return libraryPath.BaseName() + EXTENSION + libraryPath.Extension();
    }

    typedef dispatcher::Plugin_ABC* (*CreateFunctor)( dispatcher::Model_ABC&, const dispatcher::StaticModel&, dispatcher::SimulationPublisher_ABC&, dispatcher::ClientPublisher_ABC&, const dispatcher::Config&, dispatcher::Logger_ABC&, xml::xistream& );
    typedef void (*DestroyFunctor)( dispatcher::Plugin_ABC*, dispatcher::Logger_ABC& );

    template< typename T >
    T LoadFunction( HMODULE& module, const std::string& name )
    {
        T function = (T)GetProcAddress( module, name.c_str() );
        if( !function )
            throw MASA_EXCEPTION( "unable to find function '" + name + "'" );
        return function;
    }

    const unsigned int environment_buffer_size = 32767;

    struct EnvironmentGuard
    {
        explicit EnvironmentGuard( const tools::Path& directory )
        {
            memset( path_, 0, environment_buffer_size );
            if( GetEnvironmentVariableW( L"Path", path_, environment_buffer_size ) == 0 )
                throw MASA_EXCEPTION( "Failed to retrieve 'PATH' environment variable." );
            const std::wstring newPath( directory.ToUnicode() + L";" + std::wstring( path_ ) );
            if( !SetEnvironmentVariableW( L"Path", newPath.c_str() ) )
                throw MASA_EXCEPTION( "Failed to set 'PATH' environment variable." );
        }
        ~EnvironmentGuard()
        {
            if( !std::wstring( path_ ).empty() )
                SetEnvironmentVariableW( L"Path", path_ );
        }
        wchar_t path_[ environment_buffer_size ];
    };
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::LoadPlugin
// Created: SBO 2011-01-31
// -----------------------------------------------------------------------------
void PluginFactory::LoadPlugin( const tools::Path& name, xml::xistream& xis )
{
    try
    {
        const EnvironmentGuard environment( config_.BuildPluginDirectory( name ) );
        const tools::Path library = SetLibraryConfiguration( xis.attribute< tools::Path >( "library" ) );
        HMODULE module = LoadLibraryW( library.ToUnicode().c_str() );
        if( !module )
            throw MASA_EXCEPTION( "failed to load library: '" + library.ToUTF8() + "'" );
        CreateFunctor createFunction = LoadFunction< CreateFunctor >( module, "CreateInstance" );
        DestroyFunctor destroyFunction = LoadFunction< DestroyFunctor >( module, "DestroyInstance" );
        boost::shared_ptr< Logger_ABC > logger( new FileLogger( name + "_plugin.log", config_ ) );
        boost::shared_ptr< Plugin_ABC > plugin(
                createFunction( *model_, staticModel_, simulation_, *clients_, config_, *logger, xis ),
                // Note the lambda holds a reference to the logger
                [logger, destroyFunction]( dispatcher::Plugin_ABC* p )
                {
                    destroyFunction( p, *logger );
                });
        if( !plugin.get() )
            throw MASA_EXCEPTION( "CreateFunctor returned an error (see details in plugin log file)" );
        checkpointFilter_->Add( plugin );
        MT_LOG_INFO_MSG( "Plugin '" << name << "' loaded (file: " << library.ToUTF8() << ")" );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Failed to load plugin '" << name << "' reason: " << tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Failed to load plugin '" << name << "', unknown reason" );
    }
}

void PluginFactory::Receive( const std::string& link, const sword::ClientToSim& msg )
{
    dispatcher::UnicastPublisher unicaster( rights_->GetAuthenticatedPublisher( link ), link,
            rights_->GetClientID( link ), msg.context() );
    rootHandler_.HandleClientToSim( msg, unicaster, *clients_ );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DispatcherFacade.h"
#include "dispatcher/Config.h"
#include "dispatcher/Dispatcher.h"
#include "positions_plugin/PositionsPluginFactory.h"
#include "tools/Codec.h"
#include <tools/Exception.h>
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: DispatcherFacade constructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::DispatcherFacade( int argc, char** argv, int maxConnections )
{
    tools::SetCodec();
    config_.reset( new dispatcher::Config() );
    console_.reset( new MT_ConsoleLogger() );
    MT_LOG_REGISTER_LOGGER( *console_ );
    try
    {
        config_->Parse( argc, argv );
    }
    catch( const std::exception& e )
    {
        // We expect most of failures here to come from XML misconfigurations
        // rather than invalid paths generation, therefore exercises/sessions
        // paths, required to open a log file, are likely to be valid. Just
        // do that and log the error, in append mode.
        MT_FileLogger logger(
            config_->BuildSessionChildFile( "Dispatcher.log" ),
            1, 0, MT_Logger_ABC::eLogLevel_All, false, MT_Logger_ABC::eDispatcher,
            false );
        MT_LOG_REGISTER_LOGGER( logger );
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        MT_LOG_UNREGISTER_LOGGER( logger );
        throw;
    }
    bool bClearPreviousLog = !config_->HasCheckpoint();
    file_.reset( new MT_FileLogger( config_->BuildSessionChildFile( "Dispatcher.log" ),
                                    config_->GetDispatcherLogFiles(), config_->GetDispatcherLogSize(),
                                    config_->GetDispatcherLogLevel(), bClearPreviousLog,
                                    MT_Logger_ABC::eDispatcher, config_->IsDispatcherLogInBytes() ) );
    MT_LOG_REGISTER_LOGGER( *file_ );
    try
    {
        MT_LOG_INFO_MSG( "Setting max-connections=" + boost::lexical_cast< std::string >( maxConnections ) );
        dispatcher_.reset( new dispatcher::Dispatcher( *config_, maxConnections ) );
        dispatcher_->Register( *new positions::PositionsPluginFactory() );
        dispatcher_->CreatePlugins();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Dispatcher failed: " << tools::GetExceptionMsg( e ) );
        throw;
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Dispatcher failed: Unknown error" );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: DispatcherFacade destructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::~DispatcherFacade()
{
    MT_LOG_UNREGISTER_LOGGER( *file_ );
    MT_LOG_UNREGISTER_LOGGER( *console_ );
}

// -----------------------------------------------------------------------------
// Name: DispatcherFacade::Update
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
void DispatcherFacade::Update()
{
    dispatcher_->Update();
}

// -----------------------------------------------------------------------------
// Name: CreateDispatcherFacade
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void* CreateDispatcherFacade( int argc, char** argv, int maxConnections )
{
    try
    {
        return new DispatcherFacade( argc, argv, maxConnections );
    }
    catch( ... ) // $$$$ MCO 2013-01-18: it's too late to log anything because the loggers have been unregistered in ~DispatcherFacade
    {
        // NOTHING
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyDispatcherFacade
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void DestroyDispatcherFacade( void* dispatchFacade )
{
    if( dispatchFacade )
        delete static_cast< DispatcherFacade* >( dispatchFacade );
}

// -----------------------------------------------------------------------------
// Name: UpdateDispatcherFacade
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
bool UpdateDispatcherFacade( void* dispatchFacade )
{
    try
    {
        if( dispatchFacade )
            static_cast< DispatcherFacade* >( dispatchFacade )->Update();
        return true;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Updating: " << tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Updating: Unknown error" );
    }
    return false;
}

#include "dispatcher/DispatcherLoader.h"

namespace // for compilation check only
{
    static const dispatcher::DispatcherLoader::T_FacadeCreator creator = &CreateDispatcherFacade;
    static const dispatcher::DispatcherLoader::T_FacadeDestructor destructor = &DestroyDispatcherFacade;
    static const dispatcher::DispatcherLoader::T_FacadeUpdator updator = &UpdateDispatcherFacade;
}

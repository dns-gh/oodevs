// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "DispatcherFacade.h"
#include "dispatcher/Config.h"
#include "dispatcher/Dispatcher.h"
#include "positions_plugin/PositionsPluginFactory.h"
#include "tools/NullFileLoaderObserver.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: DispatcherFacade constructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::DispatcherFacade( int argc, char** argv, int maxConnections )
    : observer_( new tools::NullFileLoaderObserver() )
    , config_  ( new dispatcher::Config( *observer_ ) )
{
    MT_LOG_REGISTER_LOGGER( *new MT_ConsoleLogger() );
    config_->Parse( argc, argv );
    bool bClearPreviousLog = !config_->HasCheckpoint();
    MT_LOG_REGISTER_LOGGER( *new MT_FileLogger( config_->BuildSessionChildFile( "Dispatcher.log" ).c_str(), 
                                                config_->GetDispatcherLogFiles(), config_->GetDispatcherLogSize(),
                                                config_->GetDispatcherLogLevel(), bClearPreviousLog, MT_Logger_ABC::eDispatcher, config_->IsDispatcherLogInBytes() ) );
    try
    {
        dispatcher_.reset( new dispatcher::Dispatcher( *config_, maxConnections ) );
        dispatcher_->RegisterPluginFactory( *new positions::PositionsPluginFactory() );
        dispatcher_->CreatePlugins();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initializing: " << e.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: DispatcherFacade destructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::~DispatcherFacade()
{
    // NOTHING
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
extern "C" __declspec(dllexport) void* CreateDispatcherFacade( int argc, char** argv, int maxConnections )
{
    try
    {
        return new DispatcherFacade( argc, argv, maxConnections );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initializing: " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Initializing: Unknown error" );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DestroyDispatcherFacade
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void DestroyDispatcherFacade( void* dispatchFacade )
{
    if( dispatchFacade )
        delete static_cast< DispatcherFacade* >( dispatchFacade );
}

// -----------------------------------------------------------------------------
// Name: UpdateDispatcherFacade
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void UpdateDispatcherFacade( void* dispatchFacade )
{
    try
    {
        if( dispatchFacade )
            static_cast< DispatcherFacade* >( dispatchFacade )->Update();
    }
    catch( const std::exception& e )
    { 
        MT_LOG_ERROR_MSG( "Updating: " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Updating: Unknown error" );
    }
}

#define PLATFORM platform
#include "dispatcher/DispatcherLoader.h"

namespace // for compilation check only
{
    static const dispatcher::DispatcherLoader::T_FacadeCreator creator = &CreateDispatcherFacade;
    static const dispatcher::DispatcherLoader::T_FacadeDestructor destructor = &DestroyDispatcherFacade;
    static const dispatcher::DispatcherLoader::T_FacadeUpdator updator = &UpdateDispatcherFacade;
}

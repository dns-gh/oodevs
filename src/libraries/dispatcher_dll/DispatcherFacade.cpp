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
#include "bml_plugin/BmlPluginFactory.h"
#include "timeline_plugin/TimelinePluginFactory.h"
#include "crossbow_plugin/CrossbowPluginFactory.h"
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
    MT_LOG_REGISTER_LOGGER( *new MT_FileLogger( config_->BuildSessionChildFile( "Dispatcher.log" ).c_str(), MT_Logger_ABC::eLogLevel_All, true ) );
    try
    {
        dispatcher_.reset( new dispatcher::Dispatcher( *config_, maxConnections ) );
        dispatcher_->RegisterPluginFactory( *new timeline::TimelinePluginFactory() );
        dispatcher_->RegisterPluginFactory( *new crossbow::CrossbowPluginFactory() );
        dispatcher_->RegisterPluginFactory( *new bml::BmlPluginFactory() );
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

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
#include "bml_plugin/BmlPluginFactory.h"
#include "hla_plugin/HlaPluginFactory.h"
#include "dis_plugin/DisPluginFactory.h"
#include "tic_plugin/TicPluginFactory.h"
#ifdef CROSSBOW_PLUGIN
#   include "crossbow_plugin/CrossbowPluginFactory.h"
#endif

#include <MT/MT_Logger/MT_Logger_lib.h>

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: DispatcherFacade constructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::DispatcherFacade( int argc, char** argv, int maxConnections )
    : config_  ( new dispatcher::Config() )
{
    MT_LOG_REGISTER_LOGGER( *new MT_ConsoleLogger() );
    config_->Parse( argc, argv );
    MT_LOG_REGISTER_LOGGER( *new MT_FileLogger( config_->BuildSessionChildFile( "Dispatcher.log" ).c_str(), MT_Logger_ABC::eLogLevel_All, MT_Logger_ABC::eLogLayer_All, true ) );

    try
    {
        dispatcher_.reset( new dispatcher::Dispatcher( *config_, maxConnections ) );
        dispatcher_->RegisterPluginFactory( *new hla::HlaPluginFactory() );
        dispatcher_->RegisterPluginFactory( *new dis::DisPluginFactory() );
        dispatcher_->RegisterPluginFactory( *new tic::TicPluginFactory() );
#ifdef CROSSBOW_PLUGIN
        dispatcher_->RegisterPluginFactory( *new crossbow::CrossbowPluginFactory() );
#endif
        dispatcher_->RegisterPluginFactory( *new bml::BmlPluginFactory() );
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

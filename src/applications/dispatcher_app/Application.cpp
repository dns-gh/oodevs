// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "dispatcher/Dispatcher.h"
#include "hla_plugin/HlaPluginFactory.h"
#ifdef CROSSBOW_PLUGIN
#   include "gearth_plugin/GearthPluginFactory.h"
#   include "crossbow_plugin/CrossbowPluginFactory.h"
#endif
#include "MT/MT_Logger/MT_Logger_Lib.h"
#include <windows.h> //$$$ A VIRER

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training(tm) Dispatcher" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    config_.Parse( argc, argv );

    dispatcher_.reset( new dispatcher::Dispatcher( config_ ) );
    dispatcher_->RegisterPluginFactory( *new hla::HlaPluginFactory() );
#ifdef CROSSBOW_PLUGIN
    dispatcher_->RegisterPluginFactory( *new crossbow::CrossbowPluginFactory() );
    dispatcher_->RegisterPluginFactory( *new gearth::GearthPluginFactory() );
#endif
    dispatcher_->CreatePlugins();
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
int Application::Execute()
{
    try
    {
        while( 1 )
        {
            ::Sleep( 10 );
            dispatcher_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}


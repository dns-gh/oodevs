// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_app_pch.h"

#include "SIM_Dispatcher.h"
#include "SIM_Config.h"
#include "hla_plugin/HlaPluginFactory.h"
#ifdef CROSSBOW_PLUGIN
#   include "gearth_plugin/GearthPluginFactory.h"
#   include "crossbow_plugin/CrossbowPluginFactory.h"
#endif
#include "tools/win32/Win32Exception.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher constructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::SIM_Dispatcher( SIM_Config& config )
    : dispatcher_( config )
{
//    dispatcher_.RegisterPluginFactory( *new hla::HlaPluginFactory() );
#ifdef CROSSBOW_PLUGIN
    dispatcher_.RegisterPluginFactory( *new crossbow::CrossbowPluginFactory() );
    dispatcher_.RegisterPluginFactory( *new gearth::GearthPluginFactory() );
#endif
    dispatcher_.CreatePlugins();

    Thread::Start();
}
    
// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher destructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::~SIM_Dispatcher()
{
    Thread::Kill();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::Run
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
void SIM_Dispatcher::Run()
{
    while( 1 )
    {
        dispatcher_.Update();
        Sleep( 25 ); 
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void SIM_Dispatcher::OnUnexpected( Win32Exception& e )
{
    MT_LOG_ERROR_MSG( "Win32 Exception caught in dispatcher thread : " << e.what() );
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void SIM_Dispatcher::OnUnexpected( std::exception& e )
{
    MT_LOG_ERROR_MSG( "Exception caught in dispatcher thread : " << e.what() );
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::OnUnexpected
// Created: AGE 2005-08-22
// -----------------------------------------------------------------------------
void SIM_Dispatcher::OnUnexpected()
{
    MT_LOG_ERROR_MSG( "Unknown exception caught in dispatcher thread" );
}
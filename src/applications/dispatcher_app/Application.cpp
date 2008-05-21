// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "dispatcher_dll/DispatcherFacade.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"
#include <windows.h>

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training(tm) Dispatcher" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    dispatcher_.reset( new DispatcherFacade( argc, argv ) );
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

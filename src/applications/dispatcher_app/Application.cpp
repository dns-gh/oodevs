// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"
#include "MT_Tools/MT_Logger.h"
#include "dispatcher/DispatcherLoader.h"

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, int maxConnections )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training(tm) Dispatcher" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    dispatcher_.reset( new dispatcher::DispatcherLoader( argc, argv, maxConnections ) );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::~Application()
{
    // NOTHING
}

#pragma warning( disable : 4127 ) //conditional expression is constant

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
int Application::Execute( bool test )
{
    try
    {
        while( true )
        {
            ::Sleep( 10 );
            dispatcher_->Update();
            // do only one update in test mode
            if( test )
                break;
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Initialization error : " << e.what() );
    }
    return 0;
}

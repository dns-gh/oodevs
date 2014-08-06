// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Application.h"

#include "dispatcher/DispatcherLoader.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/IpcWatch.h"
#include <tools/WaitEvent.h>

// -----------------------------------------------------------------------------
// Name: Application constructor
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
Application::Application( int argc, char** argv, int maxConnections )
    : quit_( new tools::WaitEvent() )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Officer Training Dispatcher" );
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

// -----------------------------------------------------------------------------
// Name: Application::Run
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Application::Execute( bool test )
{
    tools::ipc::Watch watch( *quit_ );
    do{}
    while( dispatcher_->Update() && !test && !quit_->Wait( boost::posix_time::milliseconds( 10 ) ) );
}

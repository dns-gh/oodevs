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
#include "tools/win32/Win32Exception.h"

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher constructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::SIM_Dispatcher( SIM_Config& config )
    : dispatcher_( config )
{
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
      /*  do
        {
            {
                MT_CriticalSectionLocker locker( dinEngineCriticalSection_ );
                dinEngine_.Update();
            }
            Yield();
        }
        while( dinEngine_.GetEventManager().GetPendingEvents() > 10 || pConnectionMgr_->NeedsUpdating() );
        Sleep( 25 );*/
        
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
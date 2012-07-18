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
#include "MT_Tools/MT_Logger.h"
#include "dispatcher/DispatcherLoader.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher constructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::SIM_Dispatcher( int argc, char** argv, int maxConnections )
    : running_   ( true )
    , dispatcher_( new dispatcher::DispatcherLoader( argc, argv, maxConnections ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher destructor
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
SIM_Dispatcher::~SIM_Dispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::Run
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
void SIM_Dispatcher::Run()
{
    while( running_ ) // $$$$ MCO : should be protected with a mutex
    {
        dispatcher_->Update();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 25 ) ) ;
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_Dispatcher::Stop
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void SIM_Dispatcher::Stop()
{
    running_ = false ; // $$$$ MCO : should be protected with a mutex
}

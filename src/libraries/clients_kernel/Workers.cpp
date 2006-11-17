// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Workers.h"
#include <windows.h>

using namespace kernel;

namespace
{
    struct PrioritySetter
    {
        void operator()()
        {
            SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_LOWEST );
            tools::thread::Thread::Sleep( 1000 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Workers constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Workers::Workers()
    : pool_()
{
    const unsigned threads = pool_.GetCpuNumber();
    for( unsigned i = 0; i < threads; ++i )
        pool_.Enqueue( PrioritySetter() );
}

// -----------------------------------------------------------------------------
// Name: Workers destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Workers::~Workers()
{
    // NOTHING
}

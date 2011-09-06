// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MemoryLogger.h"
#include "tools/win32/ProcessMonitor.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/ClientSenders.h"
#include <ctime>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MemoryLogger constructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
MemoryLogger::MemoryLogger()
    : monitor_     ( new ProcessMonitor() )
    , next_        ( 0 )
    , nCurrentTick_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MemoryLogger destructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
MemoryLogger::~MemoryLogger()
{
    // NOTHING
}

namespace
{
    double ToMb( unsigned long long bytes )
    {
        return bytes / 1048576.;
    }
}

// -----------------------------------------------------------------------------
// Name: MemoryLogger::Update
// Created: LGY 2011-09-06
// -----------------------------------------------------------------------------
void MemoryLogger::Update( const sword::ControlEndTick& message )
{
    nCurrentTick_ = message.current_tick();
}

// -----------------------------------------------------------------------------
// Name: MemoryLogger::Update
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
void MemoryLogger::Update()
{
    const time_t current = time( 0 );
    if( current > next_ )
    {
        next_ = static_cast< int >( current ) + 60;
        monitor_->MonitorProcess();
        MT_LOG_INFO_MSG( "**************** Time tick " << nCurrentTick_ << " - Memory: " << ToMb( monitor_->GetMemory() ) << " MB / " << ToMb( monitor_->GetVirtualMemory() ) << " MB (VM)" );
    }
}

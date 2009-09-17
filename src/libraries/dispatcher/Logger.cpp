// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Logger.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <limits.h>
#include <time.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
Logger::Logger()
: nextMemoryDump_( 0 )
{
    monitor_.MonitorProcess();
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger::Update
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
void Logger::Update()
{
    time_t currentTime = time( 0 );
    if( currentTime > nextMemoryDump_ )
    {
        nextMemoryDump_ = currentTime + 60;
        MT_LOG_INFO_MSG( "Memory: " << ( monitor_.GetMemory() / 1048576.) << " MB / " << ( monitor_.GetVirtualMemory() / 1048576.) << " MB (VM)" );
    }
}

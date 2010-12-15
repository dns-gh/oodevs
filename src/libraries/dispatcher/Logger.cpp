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
#include "MT_Tools/MT_Logger.h"
#include <ctime>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
Logger::Logger()
    : next_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
Logger::~Logger()
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
// Name: Logger::Update
// Created: LDC 2009-09-09
// -----------------------------------------------------------------------------
void Logger::Update()
{
    const time_t current = time( 0 );
    if( current > next_ )
    {
        next_ = static_cast< int >( current ) + 60;
        MT_LOG_INFO_MSG( "Memory: " << ToMb( monitor_.GetMemory() ) << " MB / " << ToMb( monitor_.GetVirtualMemory() ) << " MB (VM)" );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CoreLogger.h"
#include "MT_Tools/MT_Logger.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: CoreLogger constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
CoreLogger::CoreLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoreLogger destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
CoreLogger::~CoreLogger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoreLogger::Log
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void CoreLogger::Log( core::Level level, const std::string& message ) const
{
    switch( level )
    {
    case core::INFO:
        MT_LOG_INFO_MSG( message );
        break;
    case core::WARNING:
        MT_LOG_WARNING_MSG( message );
        break;
    case core::ERROR:
        MT_LOG_ERROR_MSG( message );
        break;
    case core::FATAL:
        MT_LOG_FATAL_ERROR_MSG( message );
        break;
    }
}

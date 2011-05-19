// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "Logger.h"
#include <vlutil/vlprint.h>

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
Logger::Logger( dispatcher::Logger_ABC& logger )
    : logger_( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logger::LogInfo
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void Logger::LogInfo( const std::string& message )
{
    logger_.LogInfo( message );
    DtInfo << message << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::LogError
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void Logger::LogError( const std::string& message )
{
    logger_.LogError( message );
    DtFatal << message << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::LogWarning
// Created: SBO 2011-05-19
// -----------------------------------------------------------------------------
void Logger::LogWarning( const std::string& message )
{
    logger_.LogWarning( message );
    DtWarn << message << std::endl;
}

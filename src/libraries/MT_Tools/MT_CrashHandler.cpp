// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_CrashHandler.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/win32/StackWalkerProxy.h>

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueSearch
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueSearch( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ContinueSearch( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ExecuteHandler
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ExecuteHandler( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ExecuteHandler( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueExecution
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueExecution( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ContinueExecution( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

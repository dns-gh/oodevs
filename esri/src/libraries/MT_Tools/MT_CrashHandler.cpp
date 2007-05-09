// *****************************************************************************
//
// $Created: NLD 2006-03-17 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_CrashHandler.h"

#include "tools/win32/StackWalkerProxy.h"

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueSearch
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueSearch( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int nResult = StackWalkerProxy::ContinueSearch( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return nResult;
}

    // -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ExecuteHandler
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ExecuteHandler( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int nResult = StackWalkerProxy::ExecuteHandler( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueExecution
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueExecution( const void* exceptionInfos )
{
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int nResult = StackWalkerProxy::ContinueExecution( exceptionInfos, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return nResult;
}

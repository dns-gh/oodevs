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
#include <windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>

#pragma comment( lib, "Dbghelp.lib" )

namespace
{
    void Dump( EXCEPTION_POINTERS* pExceptionPointers )
    {
        BOOL bMiniDumpSuccessful;
        CHAR szFileName[MAX_PATH];
        HANDLE hDumpFile;
        SYSTEMTIME stLocalTime;
        MINIDUMP_EXCEPTION_INFORMATION ExpParam;

        GetLocalTime( &stLocalTime );

        StringCchPrintf( szFileName, MAX_PATH, "Debug\\SWORD-crash-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
                   stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
                   stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
                   GetCurrentProcessId(), GetCurrentThreadId());
        hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
                    FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

        if( hDumpFile == INVALID_HANDLE_VALUE )
        {
            MT_LOG_ERROR_MSG( "Crash - unable to create file " << szFileName );
            return;
        }

        GetFinalPathNameByHandle( hDumpFile, szFileName, MAX_PATH, VOLUME_NAME_DOS );

        ExpParam.ThreadId = GetCurrentThreadId();
        ExpParam.ExceptionPointers = pExceptionPointers;
        ExpParam.ClientPointers = TRUE;

        bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                        hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);
        if( bMiniDumpSuccessful )
            MT_LOG_ERROR_MSG( "Crash - dumped " << szFileName );
        if( ! bMiniDumpSuccessful )
            MT_LOG_ERROR_MSG( "Crash - unable to create dump file " << szFileName );
    }
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueSearch
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueSearch( EXCEPTION_POINTERS* pExceptionPointers )
{
    Dump( pExceptionPointers );
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ContinueSearch( pExceptionPointers, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ExecuteHandler
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ExecuteHandler( EXCEPTION_POINTERS* pExceptionPointers )
{
    Dump( pExceptionPointers );
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ExecuteHandler( pExceptionPointers, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::ContinueExecution
// Created: NLD 2006-03-17
// -----------------------------------------------------------------------------
int MT_CrashHandler::ContinueExecution( EXCEPTION_POINTERS* pExceptionPointers )
{
    Dump( pExceptionPointers );
    std::stringstream stream;
    stream << "Crash - stack trace :" << std::endl;
    int result = StackWalkerProxy::ContinueExecution( pExceptionPointers, stream );
    MT_LOG_ERROR_MSG( stream.str() );
    return result;
}

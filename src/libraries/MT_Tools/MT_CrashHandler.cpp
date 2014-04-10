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
#include <tools/Path.h>
#include <tools/win32/StackWalkerProxy.h>
#include <windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>
#include <psapi.h>

#pragma comment( lib, "Dbghelp.lib" )

namespace
{
    // bax: global var is unavoidable as exception filters have no context...
    static tools::Path rootDirectory = "Debug";

    bool Log( HANDLE hFile, BOOL bMiniDumpSuccessful )
    {
        bool bSuccess = false;
        TCHAR pszFilename[MAX_PATH+1];
        HANDLE hFileMap;

        // Get the file size.
        DWORD dwFileSizeHi = 0;
        DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi); 

        if( dwFileSizeLo == 0 && dwFileSizeHi == 0 )
            return false;

        // Create a file mapping object.
        hFileMap = CreateFileMapping(hFile, 
            NULL, 
            PAGE_READONLY,
            0, 
            1,
            NULL);

        if (hFileMap) 
        {
            // Create a file mapping to get the file name.
            void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

            if (pMem) 
            {
                if (GetMappedFileName (GetCurrentProcess(), 
                    pMem, 
                    pszFilename,
                    MAX_PATH)) 
                {

                    // Translate path with device name to drive letters.
                    TCHAR szTemp[MAX_PATH];
                    szTemp[0] = '\0';

                    if (GetLogicalDriveStrings(MAX_PATH-1, szTemp)) 
                    {
                        TCHAR szName[MAX_PATH];
                        TCHAR szDrive[3] = TEXT(" :");
                        BOOL bFound = FALSE;
                        TCHAR* p = szTemp;

                        do 
                        {
                            // Copy the drive letter to the template string
                            *szDrive = *p;

                            // Look up each device name
                            if (QueryDosDevice(szDrive, szName, MAX_PATH))
                            {
                                size_t uNameLen = _tcslen(szName);

                                if (uNameLen < MAX_PATH)
                                {
                                    bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
                                        && *(pszFilename + uNameLen) == _T('\\');

                                    if (bFound)
                                    {
                                        // Reconstruct pszFilename using szTempFile
                                        // Replace device path with DOS path
                                        TCHAR szTempFile[MAX_PATH];
                                        StringCchPrintf(szTempFile,
                                            MAX_PATH,
                                            TEXT("%s%s"),
                                            szDrive,
                                            pszFilename+uNameLen);
                                        StringCchCopyN(pszFilename, MAX_PATH+1, szTempFile, _tcslen(szTempFile));
                                    }
                                }
                            }

                            // Go to the next NULL character.
                            while (*p++);
                        } while (!bFound && *p); // end of string
                    }
                }
                bSuccess = true;
                UnmapViewOfFile(pMem);
            } 

            CloseHandle(hFileMap);
        }
        if( ! bSuccess )
            return bSuccess;
        if( bMiniDumpSuccessful )
            MT_LOG_FATAL_ERROR_MSG( "Crash - dumped " << pszFilename );
        if( ! bMiniDumpSuccessful )
            MT_LOG_FATAL_ERROR_MSG( "Crash - unable to create dump file " << pszFilename );
        return(bSuccess);
    }

    void Dump( EXCEPTION_POINTERS* pExceptionPointers )
    {
        BOOL bMiniDumpSuccessful;
        WCHAR szFileName[MAX_PATH];
        HANDLE hDumpFile;
        SYSTEMTIME stLocalTime;
        MINIDUMP_EXCEPTION_INFORMATION ExpParam;

        GetLocalTime( &stLocalTime );

        const tools::Path filename = ( rootDirectory / "SWORD-crash-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp" ).MakePreferred();
        StringCchPrintfW( szFileName, MAX_PATH, filename.ToUnicode().c_str(),
                   stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
                   stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
                   GetCurrentProcessId(), GetCurrentThreadId());
        hDumpFile = CreateFileW(szFileName, GENERIC_READ|GENERIC_WRITE, 
                    FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

        if( hDumpFile == INVALID_HANDLE_VALUE )
        {
            MT_LOG_FATAL_ERROR_MSG( "Crash - unable to create file " << szFileName );
            return;
        }

        ExpParam.ThreadId = GetCurrentThreadId();
        ExpParam.ExceptionPointers = pExceptionPointers;
        ExpParam.ClientPointers = TRUE;

        bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                        hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);
        if( ! Log( hDumpFile, bMiniDumpSuccessful ) )
        {
            if( bMiniDumpSuccessful )
                MT_LOG_FATAL_ERROR_MSG( "Crash - dumped " << szFileName );
            if( ! bMiniDumpSuccessful )
                MT_LOG_FATAL_ERROR_MSG( "Crash - unable to create dump file " << szFileName );
        }
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
    MT_LOG_FATAL_ERROR_MSG( stream.str() );
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
    MT_LOG_FATAL_ERROR_MSG( stream.str() );
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
    MT_LOG_FATAL_ERROR_MSG( stream.str() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MT_CrashHandler::SetRootDirectory
// Created: BAX 2012-08-08
// -----------------------------------------------------------------------------
void MT_CrashHandler::SetRootDirectory( const tools::Path& root )
{
    rootDirectory = root;
}

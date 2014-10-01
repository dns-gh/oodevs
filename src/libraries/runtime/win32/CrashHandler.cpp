// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime/CrashHandler.h"

#include <windows.h> // must be included first
#include <dbghelp.h>
#include <shlwapi.h>
#include <stdio.h>
#include <time.h>

using namespace runtime;

namespace
{
static LONG g_access;

#define COUNT_OF( X ) (sizeof(X)/sizeof*(X))

// disable all further calls to SetUnhandledExceptionFilter
#ifndef _WIN64
static const BYTE g_func[] =
{
    0x33, 0xC0,       // xor eax eax
    0xC2, 0x04, 0x00, // ret 4
};
#else
static const BYTE g_func[] =
{
   0x33, 0xC0,       // xor eax eax
   0XC3,             // ret
};
#endif
static BYTE g_save[ COUNT_OF( g_func ) ];

int WriteMemory( void* dst, const void* src, size_t size )
{
   if( !dst || !src )
       return 0;

   DWORD done, save;
   done = VirtualProtect( dst, size, PAGE_EXECUTE_READWRITE, &save );
   if( !done )
       return 0;

   memcpy(dst, src, size);
   return VirtualProtect(dst, size, save, &done);
}

int PatchExceptionFilter( bool patch )
{
    HMODULE hlib = GetModuleHandleA( "kernel32.dll" );
    if( !hlib )
        return 0;

    void* dst = GetProcAddress( hlib, "SetUnhandledExceptionFilter" );
    if( !dst )
        return 0;
    if( !patch )
        return WriteMemory( dst, g_save, sizeof g_save );
    else
        return WriteMemory( g_save, dst, sizeof g_save )
            && WriteMemory( dst, g_func, sizeof g_func );
}

HMODULE LoadModule( const wchar_t* module )
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW( NULL, path, MAX_PATH );
    PathRemoveFileSpecW( path );
    PathAppendW( path, module );
    HMODULE tmp = LoadLibraryW( path );
    if( !tmp )
        tmp = LoadLibraryW( module );
    return tmp;
}

void GetTemporaryFilename( wchar_t* path, int size )
{
    time_t current = time( NULL );
    struct tm tmp;
    localtime_s( &tmp, &current );

    wchar_t timestamp[32];
    wcsftime( timestamp, COUNT_OF( timestamp ), L"-%y%m%d_%H%M%S", &tmp );

    GetModuleFileNameW( 0, path, size );
    PathRemoveExtensionW( path );
    wcscat_s( path, size, timestamp );
    PathAddExtensionW( path, L".dmp" );
}

typedef BOOL (WINAPI* MINIDUMPWRITEDUMP)(HANDLE process, DWORD pid, HANDLE file, MINIDUMP_TYPE type,
                                         CONST PMINIDUMP_EXCEPTION_INFORMATION   exception_param,
                                         CONST PMINIDUMP_USER_STREAM_INFORMATION user_stream_param,
                                         CONST PMINIDUMP_CALLBACK_INFORMATION    callback_param);

void MakeDump( PEXCEPTION_POINTERS exception, MINIDUMP_TYPE type )
{
    PatchExceptionFilter( false );
    SetUnhandledExceptionFilter( NULL );
    HMODULE dbghelp = LoadModule( L"dbghelp.dll" );
    if( !dbghelp )
        return;

    MINIDUMPWRITEDUMP dumper = (MINIDUMPWRITEDUMP) GetProcAddress( dbghelp, "MiniDumpWriteDump" );
    if( !dumper )
        return;

    wchar_t path[MAX_PATH];
    for( size_t retry = 0; retry < 20; ++retry )
    {
        GetTemporaryFilename( path, MAX_PATH );
        if( !PathFileExistsW( path ) )
            break;
        Sleep( 1000 );
    }

    HANDLE file = CreateFileW( path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( file == INVALID_HANDLE_VALUE )
        return;

    MINIDUMP_EXCEPTION_INFORMATION info;
    info.ThreadId = GetCurrentThreadId();
    info.ExceptionPointers = exception;
    info.ClientPointers = 0;
    dumper( GetCurrentProcess(), GetCurrentProcessId(), file, type, &info, NULL, NULL );
    CloseHandle( file );
}

LONG WINAPI TopLevelFilter( PEXCEPTION_POINTERS exception )
{
    MakeDump( exception, MiniDumpNormal );
    ExitProcess( INT_MAX );
}
}

int runtime::CrashHandlerInit()
{
    LONG first = InterlockedIncrement( &g_access );
    if( first != 1 )
        return 0;
    SetUnhandledExceptionFilter( TopLevelFilter );
    return PatchExceptionFilter( true );
}

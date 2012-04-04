// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Api.h"
#include "runtime/Utf8.h"
#include <stdexcept>
#include <psapi.h>
#if ( PSAPI_VERSION == 1 )
    #pragma comment( lib, "psapi.lib" )
#endif

using namespace runtime;

// -----------------------------------------------------------------------------
// Name: Api::Api
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Api::Api()
    : exit_ ( 0 )
{
    HINSTANCE kernel = 0;
    BOOL done = GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"Kernel32", &kernel );
    if( !done )
        throw std::runtime_error( "unable to load kernel32 module" );
    FARPROC exit = GetProcAddress( kernel, "ExitProcess" );
    if( !exit )
        throw std::runtime_error( "unable to get Kernel32::ExitProcess address" );
    exit_ = reinterpret_cast< LPTHREAD_START_ROUTINE >( &exit );
}

// -----------------------------------------------------------------------------
// Name: Api::~Api
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Api::~Api()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Api::GetLastError
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
std::string Api::GetLastError() const
{
    LPWSTR buffer;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER
                 | FORMAT_MESSAGE_FROM_SYSTEM
                 | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                   ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   reinterpret_cast< LPWSTR >( &buffer ), 0, NULL );
    const std::wstring error( buffer );
    LocalFree( buffer );
    return Utf8Convert( error );
}

// -----------------------------------------------------------------------------
// Name: Api::EnumProcesses
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::EnumProcesses( DWORD* ids, int cb, DWORD* pBytesReturned ) const
{
    return !!::EnumProcesses( ids, cb, pBytesReturned );
}

// -----------------------------------------------------------------------------
// Name: Api::CreateProcess
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::CreateProcess( const wchar_t* app, wchar_t* args, SECURITY_ATTRIBUTES* lpProcessAttributes, SECURITY_ATTRIBUTES* lpThreadAttributes, bool bInheritHandles, int dwCreationFlags, void* lpEnvironment, const wchar_t* lpCurrentDirectory, STARTUPINFOW* lpStartupInfo, PROCESS_INFORMATION* lpProcessInformation ) const
{
    return !!::CreateProcessW( app, args, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation );
}

// -----------------------------------------------------------------------------
// Name: Api::CloseHandle
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::CloseHandle( HANDLE hObject ) const
{
    return !!::CloseHandle( hObject );
}

// -----------------------------------------------------------------------------
// Name: Api::OpenProcess
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
HANDLE Api::OpenProcess( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const
{
    return ::OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessId );
}

// -----------------------------------------------------------------------------
// Name: Api::GetProcessName
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
int Api::GetProcessName( HANDLE hProcess, wchar_t* lpImageFileName, int nSize ) const
{
    return ::GetProcessImageFileNameW( hProcess, lpImageFileName, nSize );
}

// -----------------------------------------------------------------------------
// Name: Api::WaitForSingleObjectEx
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
int Api::WaitForSingleObjectEx( HANDLE hHandle, int dwMilliseconds, bool bAlertable ) const
{
    return ::WaitForSingleObjectEx( hHandle, dwMilliseconds, bAlertable );
}

// -----------------------------------------------------------------------------
// Name: Api::TerminateProcess
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::TerminateProcess( HANDLE hProcess, unsigned uExitCode ) const
{
    return !!::TerminateProcess( hProcess, uExitCode );
}

// -----------------------------------------------------------------------------
// Name: Api::GetExitCodeProcess
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::GetExitCodeProcess( HANDLE hProcess, DWORD* lpExitCode ) const
{
    return !!::GetExitCodeProcess( hProcess, lpExitCode );
}

// -----------------------------------------------------------------------------
// Name: Api::CreateRemoteThreadExt
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
HANDLE Api::CreateRemoteThreadExt( HANDLE hProcess, SECURITY_ATTRIBUTES* lpThreadAttributes, size_t dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, int dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD* lpThreadId ) const
{
    return ::CreateRemoteThreadEx( hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpAttributeList, lpThreadId );
}

// -----------------------------------------------------------------------------
// Name: Api::GetExitProcessPointer
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
LPTHREAD_START_ROUTINE Api::GetExitProcessPointer() const
{
    return exit_;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Api.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"
#include <cpplog/cpplog.hpp>

#include <stdexcept>
#include <windows.h>
#include <psapi.h>
#if ( PSAPI_VERSION == 1 )
    #pragma comment( lib, "psapi.lib" )
#endif

using namespace runtime;

#define LOG_AND_THROW( message ) {\
    LOG_ERROR( log_ ) << "[win32] " << GetLastError();\
    throw std::runtime_error( message );\
}

// -----------------------------------------------------------------------------
// Name: Api::Api
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Api::Api( cpplog::BaseLogger& log )
    : log_ ( log )
{
    // NOTHING
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
    return Utf8( error );
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
    bool reply = !!::TerminateProcess( hProcess, uExitCode );
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] " << GetLastError();
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] Unable to terminate process";
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Api::GetModuleFilename
// Created: BAX 2012-04-24
// -----------------------------------------------------------------------------
std::wstring Api::GetModuleFilename() const
{
    DWORD ret;
    std::vector< wchar_t > buffer( 256 );
    for(;;)
    {
        ret = ::GetModuleFileNameW( 0, &buffer[0], static_cast< DWORD >( buffer.size() ) );
        if( ret < static_cast< DWORD >( buffer.size() ) || ::GetLastError() != ERROR_INSUFFICIENT_BUFFER )
            break;
        buffer.resize( buffer.size() * 2 );
    }
    return std::wstring( &buffer[0], ret );
}

// -----------------------------------------------------------------------------
// Name: Api::GetCurrentProcessIdentifier
// Created: PMD 2012-09-14
// -----------------------------------------------------------------------------
int Api::GetCurrentProcessIdentifier() const
{
    return ::GetCurrentProcessId();
}

bool Api_ABC::Has64BitSystem()
{
#ifdef _WIN64
 return true;
#else
 bool x64 = false;
 return IsWow64Process( GetCurrentProcess(), &x64 ) && x64;
#endif
}

bool Api_ABC::Is64BitBinary( const wchar_t* app )
{
    DWORD type;
    int err = GetBinaryTypeW( app, &type );
    return err && type == SCS_64BIT_BINARY;
}

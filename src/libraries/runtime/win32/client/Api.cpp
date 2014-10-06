// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "../Api.h"
#include "runtime/Scoper.h"
#include <cpplog/cpplog.hpp>
#include <boost/bind.hpp>
#include <windows.h>

using namespace runtime;

#define LOG_AND_THROW( message ) {\
    LOG_ERROR( log_ ) << "[win32] " << GetLastError();\
    throw std::runtime_error( message );\
}

namespace
{
void TryCloseHandle( HANDLE handle )
{
    if( handle )
        CloseHandle( handle );
}

HANDLE MakeFileHandle( const wchar_t* file )
{
    SECURITY_ATTRIBUTES sec = {};
    sec.nLength = sizeof sec;
    sec.bInheritHandle = true;
    HANDLE reply = CreateFileW( file, FILE_APPEND_DATA, FILE_SHARE_READ, &sec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( reply == INVALID_HANDLE_VALUE )
        return NULL;
    DWORD dummy;
    if( GetLastError() == ERROR_ALREADY_EXISTS )
        WriteFile( reply, "\n", 1, &dummy, 0 );
    return reply;
}

HANDLE MakeProcess( const wchar_t* app, wchar_t* args, const wchar_t* run, STARTUPINFOW* ex, bool inherit, int& pid )
{
    PROCESS_INFORMATION info = {};
    int flags = NORMAL_PRIORITY_CLASS | DETACHED_PROCESS;
    bool valid = !!CreateProcessW( app, args, 0, 0, inherit, flags, 0, run, ex, &info );
    if( !valid )
        return NULL;
    Scoper thread( boost::bind( &TryCloseHandle, info.hThread ) );
    pid = info.dwProcessId;
    return info.hProcess;
}

HANDLE MakeProcess( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log, int& pid )
{
    STARTUPINFOW info = { sizeof info };
    pid = 0;
    if( !log )
        return MakeProcess( app, args, run, &info, false, pid );

    HANDLE inherits[2] = {};
    inherits[0] = MakeFileHandle( log );
    if( !inherits[0] )
        return NULL;
    Scoper out( boost::bind( &TryCloseHandle, inherits[0] ) );

    const bool valid = !!DuplicateHandle( GetCurrentProcess(), inherits[0], GetCurrentProcess(), &inherits[1], 0, true, DUPLICATE_SAME_ACCESS );
    if( !valid )
        return NULL;
    Scoper err( boost::bind( &TryCloseHandle, inherits[1] ) );

    info.dwFlags = STARTF_USESTDHANDLES;
    info.hStdOutput = inherits[0];
    info.hStdError = inherits[1];
    return ::MakeProcess( app, args, run, &info, true, pid );
}
}

// -----------------------------------------------------------------------------
// Name: Api::MakeProcess
// Created: BAX 2012-05-15
// -----------------------------------------------------------------------------
ProcessDescriptor Api::MakeProcess( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log ) const
{
    ProcessDescriptor proc;
    proc.handle = ::MakeProcess( app, args, run, log, proc.pid );
    if( !proc.handle )
    {
        LOG_ERROR( log_ ) << "[win32] " << GetLastError();
        LOG_ERROR( log_ ) << "[win32] Unable to create process";
    }
    return proc;
}

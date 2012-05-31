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
#include <cpplog/cpplog.hpp>

#include <stdexcept>
#include <windows.h>
#include <psapi.h>
#if ( PSAPI_VERSION == 1 )
    #pragma comment( lib, "psapi.lib" )
#endif

#if defined(_MSC_VER) && !defined(_WIN64)
#define  BOOST_BIND_ENABLE_STDCALL
#endif
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
    , exit_( 0 )
{
    HINSTANCE kernel = 0;
    BOOL done = GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"Kernel32", &kernel );
    if( !done )
        LOG_AND_THROW( "unable to load kernel32 module" );

    FARPROC exit = GetProcAddress( kernel, "ExitProcess" );
    if( !exit )
        LOG_AND_THROW( "unable to get Kernel32::ExitProcess address" );
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
bool Api::EnumProcesses( unsigned long* ids, int cb, unsigned long* pBytesReturned ) const
{
    bool reply = !!::EnumProcesses( ids, cb, pBytesReturned );
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] " << GetLastError();
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] Unable to list processes";
    return reply;
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
// Name: Api::GetExitCodeProcess
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
bool Api::GetExitCodeProcess( void* hProcess, unsigned long* lpExitCode ) const
{
    return !!::GetExitCodeProcess( hProcess, lpExitCode );
}

// -----------------------------------------------------------------------------
// Name: Api::CreateRemoteThreadExt
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
HANDLE Api::CreateRemoteThreadExt( void* hProcess, size_t dwStackSize, void* lpStartAddress, void* lpParameter, int dwCreationFlags, unsigned long* lpThreadId ) const
{
    HANDLE reply = ::CreateRemoteThreadEx( hProcess, 0, dwStackSize, reinterpret_cast< LPTHREAD_START_ROUTINE >( lpStartAddress ),
                                           lpParameter, dwCreationFlags, 0, lpThreadId );
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] " << GetLastError();
    LOG_IF_NOT( ERROR, log_, reply ) << "[win32] Unable to create remote thread";
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Api::GetExitProcessPointer
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
void* Api::GetExitProcessPointer() const
{
    return exit_;
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
        if( ret < buffer.size() || ::GetLastError() != ERROR_INSUFFICIENT_BUFFER )
            break;
        buffer.resize( buffer.size() * 2 );
    }
    return std::wstring( &buffer[0], ret );
}

namespace
{
#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

struct Scoper
{
    typedef boost::function< void() > Task;
    Scoper( const Task& task )
        : task_( task )
    {
        // NOTHING
    }
    ~Scoper()
    {
        task_();
    }
private:
    Task task_;
};

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
    HANDLE reply = CreateFileW( file, GENERIC_WRITE, FILE_SHARE_READ, &sec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    return reply == INVALID_HANDLE_VALUE ? NULL : reply;
}

HANDLE MakeProcess( const wchar_t* app, wchar_t* args, const wchar_t* run, STARTUPINFOW* ex, int& pid )
{
    PROCESS_INFORMATION info = {};
    bool extended = ex->cb == sizeof( STARTUPINFOEXW );
    int flags = NORMAL_PRIORITY_CLASS | DETACHED_PROCESS;
    if( extended )
        flags |= EXTENDED_STARTUPINFO_PRESENT;
    bool valid = !!CreateProcessW( app, args, 0, 0, extended, flags, 0, run, ex, &info );
    if( !valid )
        return NULL;
    Scoper thread( boost::bind( &TryCloseHandle, info.hThread ) );
    pid = info.dwProcessId;
    return info.hProcess;
}

HANDLE MakeProcess( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log, int& pid )
{
    pid = 0;
    if( !log )
    {
        STARTUPINFOW info = { sizeof info };
        return ::MakeProcess( app, args, run, &info, pid );
    }

    HANDLE inherits[2] = {};
    SIZE_T size = 0;
    LPPROC_THREAD_ATTRIBUTE_LIST attributes = NULL;
    bool valid = ::InitializeProcThreadAttributeList( NULL, COUNT_OF( inherits ), 0, &size )
              || ::GetLastError() == ERROR_INSUFFICIENT_BUFFER;
    if( !valid )
        return NULL;

    attributes = reinterpret_cast< LPPROC_THREAD_ATTRIBUTE_LIST >( HeapAlloc( GetProcessHeap(), 0, size ) );
    if( !attributes )
        return NULL;
    Scoper freeAttributes( boost::bind( &HeapFree, GetProcessHeap(), 0, attributes ) );

    valid = !!InitializeProcThreadAttributeList( attributes, COUNT_OF( inherits ), 0, &size );
    if( !valid )
        return NULL;

    inherits[0] = MakeFileHandle( log );
    if( !inherits[0] )
        return NULL;
    Scoper out( boost::bind( &TryCloseHandle, inherits[0] ) );

    valid = !!DuplicateHandle( GetCurrentProcess(), inherits[0], GetCurrentProcess(), &inherits[1], 0, true, DUPLICATE_SAME_ACCESS );
    if( !valid )
        return NULL;
    Scoper err( boost::bind( &TryCloseHandle, inherits[1] ) );

    valid = !!UpdateProcThreadAttribute( attributes, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inherits, sizeof inherits, NULL, NULL );
    if( !valid )
        return NULL;
    Scoper clearAttributes( boost::bind( DeleteProcThreadAttributeList, attributes ) );

    STARTUPINFOEXW ex = {};
    ex.lpAttributeList = attributes;
    ex.StartupInfo.cb = sizeof ex;
    ex.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    ex.StartupInfo.hStdOutput = inherits[0];
    ex.StartupInfo.hStdError = inherits[1];
    return ::MakeProcess( app, args, run, &ex.StartupInfo, pid );
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

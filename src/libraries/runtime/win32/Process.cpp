// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Process.h"
#include "Api_ABC.h"
#include <runtime/Utf8.h>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/static_assert.hpp>

#include <windows.h>

using namespace runtime;

BOOST_STATIC_ASSERT( sizeof(Handle::element_type*) == sizeof(HANDLE) );

// -----------------------------------------------------------------------------
// Name: runtime::MakeHandle
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
Handle runtime::MakeHandle( const Api_ABC& api, void* value )
{
    return value ? Handle( value, boost::bind( &Api_ABC::CloseHandle, &api, _1 ) ) : Handle();
}

namespace
{
    Handle GetHandle( const Api_ABC& api, int pid )
    {
        int flags = 0;
        flags |= PROCESS_QUERY_INFORMATION; // CreateRemoteThread
        flags |= SYNCHRONIZE;               // WaitForSingleObjectEx
        flags |= PROCESS_TERMINATE;         // TerminateProcess
        flags |= PROCESS_CREATE_THREAD;     // CreateRemoteThread
        flags |= PROCESS_VM_OPERATION;      // CreateRemoteThread
        flags |= PROCESS_VM_WRITE;          // CreateRemoteThread
        flags |= PROCESS_VM_READ;           // CreateRemoteThread
        HANDLE handle = api.OpenProcess( flags, false, pid );
        if( !handle )
            throw std::runtime_error( ( boost::format( "unable to open process %1%" ) % pid ).str() );
        return runtime::MakeHandle( api, handle );
    }

    std::string GetName( const Api_ABC& api, Handle handle )
    {
        wchar_t name[MAX_PATH];
        int size = api.GetProcessName( handle.get(), name, sizeof name - 1 );
        return size ? Utf8( std::wstring( name ) ) : "";
    }
}

// -----------------------------------------------------------------------------
// Name: Process::Process
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Process::Process( const Api_ABC& api, int pid )
    : api_   ( api )
    , pid_   ( pid )
    , handle_( GetHandle( api_, pid_ ) )
    , name_  ( ::GetName( api_, handle_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Process::Process
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Process::Process( const Api_ABC& api, int pid, Handle handle )
    : api_   ( api )
    , pid_   ( pid )
    , handle_( handle )
    , name_  ( ::GetName( api_, handle_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Process::~Process
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Process::~Process()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Process::GetPid
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
int Process::GetPid() const
{
    return pid_;
}

// -----------------------------------------------------------------------------
// Name: Process::GetName
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
const std::string& Process::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Process::IsAlive
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
bool Process::IsAlive() const
{
    int reply = api_.WaitForSingleObjectEx( handle_.get(), 0, false );
    return reply == WAIT_TIMEOUT;
}

// -----------------------------------------------------------------------------
// Name: Process::Join
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
bool Process::Join( int msTimeout )
{
    int reply = api_.WaitForSingleObjectEx( handle_.get(), msTimeout, false );
    return reply == WAIT_OBJECT_0;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: UnsafeTerminate
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
bool UnsafeTerminate( const Api_ABC& api, HANDLE process )
{
    return api.TerminateProcess( process, static_cast< unsigned >( -1 ) );
}
}

// -----------------------------------------------------------------------------
// Name: Process::Kill
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
bool Process::Kill()
{
    return UnsafeTerminate( api_, handle_.get() );
}

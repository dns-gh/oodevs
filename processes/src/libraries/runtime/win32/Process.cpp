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
#include "Handle.h"
#include "Utf8.h"

using namespace process;

// -----------------------------------------------------------------------------
// Name: Process::Process
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Process::Process( const Api_ABC& api, int pid )
    : api_( api )
    , pid_( pid )
{
    int flags = 0;
    flags |= PROCESS_QUERY_INFORMATION; // CreateRemoteThread
    flags |= SYNCHRONIZE;               // WaitForSingleObjectEx
    flags |= PROCESS_TERMINATE;         // TerminateProcess
    flags |= PROCESS_CREATE_THREAD;     // CreateRemoteThread
    flags |= PROCESS_VM_OPERATION;      // CreateRemoteThread
    flags |= PROCESS_VM_WRITE;          // CreateRemoteThread
    flags |= PROCESS_VM_READ;           // CreateRemoteThread
    HANDLE handle = api_.OpenProcess( flags, false, pid_ );
    handle_ = std::auto_ptr< Handle >( new Handle( api_, handle ) );
    FetchName();
}

// -----------------------------------------------------------------------------
// Name: Process::Process
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Process::Process( const Api_ABC& api, int pid, std::auto_ptr< Handle > handle )
    : api_   ( api )
    , pid_   ( pid )
    , handle_( handle )
{
    FetchName();
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
// Name: Process::FetchName
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
void Process::FetchName()
{
    wchar_t name[1<<8];
    int size = api_.GetProcessName( handle_->value_, name, sizeof name - 1 );
    if( size )
        try
        {
            name_ = Utf8Convert( name );
        }
        catch( const std::exception& )
        {
            // NOTHING
        }
}

// -----------------------------------------------------------------------------
// Name: Process::Join
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
bool Process::Join( int msTimeout )
{
    int reply = api_.WaitForSingleObjectEx( handle_->value_, msTimeout, false );
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

    // -----------------------------------------------------------------------------
    // Name: SafeTerminate
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    bool SafeTerminate( const Api_ABC& api, HANDLE process, int msTimeout )
    {
        // see http://drdobbs.com/184416547 for rationale
        DWORD code = 0;
        bool done = api.GetExitCodeProcess( process, &code );
        if( !done || code != STILL_ACTIVE )
            return false;

        DWORD threadId;
        LPTHREAD_START_ROUTINE exit = api.GetExitProcessPointer();
        HANDLE thread = api.CreateRemoteThreadExt( process, 0, 0, exit, &code, 0, 0, &threadId );
        if( !thread )
            return false;

        Handle dispose( api, thread );
        int reply = api.WaitForSingleObjectEx( process, msTimeout, false );
        return reply == WAIT_OBJECT_0;
    }
}

// -----------------------------------------------------------------------------
// Name: Process::Kill
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
bool Process::Kill( int msTimeout )
{
    bool done = SafeTerminate( api_, handle_->value_, msTimeout );
    return done ? true : UnsafeTerminate( api_, handle_->value_ );
}

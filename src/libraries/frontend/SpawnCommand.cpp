// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "SpawnCommand.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include "MT_Tools\MT_Logger.h"
#include <windows.h>
#include <tlhelp32.h>

using namespace frontend;

struct SpawnCommand::InternalData
{
    PROCESS_INFORMATION pid_ ;
    InternalData() { ZeroMemory( &pid_ , sizeof( pid_ ) ); }
};

#ifndef MAKEULONGLONG
    #define MAKEULONGLONG( ldw, hdw ) ( ( ULONGLONG( hdw ) << 32 ) | ( ( ldw ) & 0xFFFFFFFF ) )
#endif

#ifndef MAXULONGLONG
    #define MAXULONGLONG ( ( ULONGLONG ) ~ ( ( ULONGLONG ) 0 ) )
#endif

// -----------------------------------------------------------------------------
// Name: SpawnCommand constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::SpawnCommand( const tools::GeneralConfig& config, const char* exe, bool attach ,
                            std::string commanderEndpoint )
    : config_                  ( config )
    , internal_                ( new InternalData() )
    , attach_                  ( attach )
    , forceProcessStop_        ( true )
    , workingDirectory_        ( "." )
    , stopped_                 ( false )
    , networkCommanderEndpoint_( commanderEndpoint )
{
    AddArgument( exe );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand constructor
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
SpawnCommand::SpawnCommand( const tools::GeneralConfig& config, DWORD processId, bool attach /*= false*/,
                            std::string commanderEndpoint /*= ""*/ )
    : config_                  ( config )
    , internal_                ( new InternalData() )
    , attach_                  ( attach )
    , forceProcessStop_        ( true )
    , workingDirectory_        ( "." )
    , stopped_                 ( false )
    , networkCommanderEndpoint_( commanderEndpoint )
{
    FillProcessInformation( processId );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::~SpawnCommand()
{
    if( attach_ )
        StopProcess();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddRootDirArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddRootDirArgument()
{
    AddArgument( ( "--root-dir=\"" + config_.GetRootDir() + "\"" ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddExerciseArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddExerciseArgument( const QString& exercise )
{
    AddArgument( "--exercise=\"" + exercise +"\"" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddSessionArgument
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddSessionArgument( const QString& session )
{
    AddArgument( "--session=\"" + session +"\"" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Start
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void SpawnCommand::Start()
{
    STARTUPINFO startupInfo = { sizeof( STARTUPINFOA ), 0, 0, 0,
        (ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT,
        0, 0, 0,
        0,
        0, 0, 0,
        0, 0, 0
    };
    std::string debug( commandLine_.local8Bit().data() ) ;
    if( !CreateProcessA( 0,                                     // lpApplicationName
                         commandLine_.local8Bit().data(),       // lpCommandLine
                         0,                    // lpProcessAttributes
                         0,                                     // lpThreadAttributes
                         FALSE,                                 // bInheritHandles
                         CREATE_NEW_CONSOLE,                    // dwCreationFlags
                         0,                                     // lpEnvironment
                         workingDirectory_.c_str(),             // lpCurrentDirectory
                         &startupInfo,                          // lpStartupInfo
                         &internal_->pid_) )                    // lpProcessInformation
    {
        DWORD errCode = GetLastError();
        throw std::exception( tools::translate( "SpawnCommand", "Could not start process: %1, error: %2" ).arg( debug.c_str() ).arg( errCode ).toAscii().constData() );
    }
}

namespace
{
    BOOL CALLBACK CloseWndProc( HWND hwnd, LPARAM /*lParam*/ )
    {
        if( IsWindow( hwnd ) )
        {
            DWORD_PTR result;
            SendMessageTimeout( hwnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 100, &result );
        }
        return TRUE;
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Attach
// Created: SBO 2010-11-05
// -----------------------------------------------------------------------------
void SpawnCommand::Attach( boost::shared_ptr< Process_ABC > process )
{
    attachment_ = process;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::StopProcess
// Created: SBO 2010-11-05
// -----------------------------------------------------------------------------
bool SpawnCommand::StopProcess()
{
    if( internal_.get() && internal_->pid_.hProcess )
    {
        HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
        THREADENTRY32 te32;

        MT_LOG_INFO_MSG( "Spawn Command : Stopping process" );
        // Take a snapshot of all running threads
        hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, internal_->pid_.dwThreadId );
        if( hThreadSnap == INVALID_HANDLE_VALUE )
        {            
            MT_LOG_INFO_MSG( "Spawn Command : Invalid snapshot" );
            return false;
        }
        te32.dwSize = sizeof( THREADENTRY32 );
        if( !Thread32First( hThreadSnap, &te32 ) )
        {
            MT_LOG_INFO_MSG( "Spawn Command : No first thread" );
            CloseHandle( hThreadSnap ); // Must clean up the snapshot object!
            return false;
        }
        do
        {
            if( te32.th32OwnerProcessID == internal_->pid_.dwProcessId )
                EnumThreadWindows( te32.th32ThreadID, &::CloseWndProc, 0 );
        }
        while( Thread32Next( hThreadSnap, &te32 ) );
        EnumThreadWindows( internal_->pid_.dwThreadId, &::CloseWndProc, 0 );
        MT_LOG_INFO_MSG( "Spawn Command : Calling TerminateProcess" );
        int failure = TerminateProcess( internal_->pid_.hProcess, 0 );
        if( failure )
        {
            MT_LOG_INFO_MSG( "Spawn Command : TerminateProcess Failure" );
            return false;
        }
        CloseHandle( hThreadSnap );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::FillProcessInformation
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
void SpawnCommand::FillProcessInformation( unsigned long processId )
{
    internal_->pid_.dwProcessId = processId;
    internal_->pid_.hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processId );
    if( internal_->pid_.hProcess == NULL )
        return;
    // Find primary thread
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;
    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
    if( hThreadSnap == INVALID_HANDLE_VALUE )
        return;
    te32.dwSize = sizeof( THREADENTRY32 );
    if( !Thread32First( hThreadSnap, &te32 ) )
    {
        CloseHandle( hThreadSnap );
        return;
    }
    ULONGLONG ullMinCreateTime = MAXULONGLONG;
    do
    {
        if( te32.th32OwnerProcessID == processId )
        {
            HANDLE hThread = OpenThread( THREAD_QUERY_INFORMATION, TRUE, te32.th32ThreadID );
            if( hThread )
            {
                FILETIME afTimes[ 4 ] = { 0 };
                if( GetThreadTimes( hThread, &afTimes[ 0 ], &afTimes[ 1 ], &afTimes[ 2 ], &afTimes[ 3 ] ) )
                {
                    ULONGLONG ullTest = MAKEULONGLONG( afTimes[ 0 ].dwLowDateTime, afTimes[ 0 ].dwHighDateTime );
                    if( ullTest && ullTest < ullMinCreateTime )
                    {
                        ullMinCreateTime = ullTest;
                        internal_->pid_.dwThreadId = te32.th32ThreadID;
                    }
                }
                CloseHandle(hThread);
            }
        }
    }
    while( Thread32Next( hThreadSnap, &te32 ) );
    CloseHandle( hThreadSnap );
    if( internal_->pid_.dwThreadId )
        internal_->pid_.hThread = OpenThread( THREAD_ALL_ACCESS, FALSE, internal_->pid_.dwThreadId );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Stop
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
void SpawnCommand::Stop( bool forceProcessStop/* = true */)
{
    forceProcessStop_ = forceProcessStop;
    stopped_ = true;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Wait
// Created: RDS 2008-08-25
// -----------------------------------------------------------------------------
bool SpawnCommand::Wait()
{
    if( !stopped_ && internal_->pid_.hProcess )
    {
        const DWORD result = WaitForSingleObject( internal_->pid_.hProcess, 100 );
        if( result != WAIT_OBJECT_0 )
        {
            ::Sleep( 100 );
            return true;
        }
    }
    if( attach_ || forceProcessStop_ )
    {
        bool stopped = StopProcess();
        if( !stopped )
        {
            ::Sleep( 100 );
            return true;
        }
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::IsRunning
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool SpawnCommand::IsRunning() const
{
    if( internal_->pid_.dwProcessId )
        return CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, internal_->pid_.dwProcessId ) != INVALID_HANDLE_VALUE;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int SpawnCommand::GetPercentage() const
{
    return IsRunning() ? 100 : 0;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::addArgument
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
void SpawnCommand::AddArgument( const QString& arg )
{
    if( !commandLine_.isEmpty() )
        commandLine_ += ' ';
    commandLine_ += arg;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString SpawnCommand::GetStatus() const
{
    if( !IsRunning() )
        return tools::translate( "SpawnCommand", "Starting..." );
    return tools::translate( "SpawnCommand", "Started" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::SetWorkingDirectory
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void SpawnCommand::SetWorkingDirectory( const QString& directory )
{
    workingDirectory_ = directory.toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetCommanderEndpoint
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const std::string& SpawnCommand::GetCommanderEndpoint() const
{
    return networkCommanderEndpoint_;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::SetCommanderEndpoint
// Created: JSR 2011-12-13
// -----------------------------------------------------------------------------
void SpawnCommand::SetCommanderEndpoint( const std::string& endpoint )
{
    networkCommanderEndpoint_ = endpoint;
}

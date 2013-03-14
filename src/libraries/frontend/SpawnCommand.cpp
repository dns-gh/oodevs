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
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include <windows.h>
#include <tlhelp32.h>

using namespace frontend;

struct SpawnCommand::InternalData
{
    PROCESS_INFORMATION pid_ ;
    InternalData() { ZeroMemory( &pid_ , sizeof( pid_ ) ); }
};

// -----------------------------------------------------------------------------
// Name: SpawnCommand constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::SpawnCommand( const tools::GeneralConfig& config, const tools::Path& exe, bool attach ,
                            std::string commanderEndpoint /* = ""*/, std::string jobName /* = ""*/ )
    : config_                   ( config )
    , internal_                 ( new InternalData() )
    , attach_                   ( attach )
    , workingDirectory_         ( "." )
    , stopped_                  ( false )
    , networkCommanderEndpoint_ ( commanderEndpoint )
    , jobName_                  ( jobName )
{
    AddArgument( exe.ToUTF8().c_str() );
    if( jobName_ == "launcher-job" )
        AddArgument( "--silent" );
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
    AddArgument( ( "--root-dir=\"" + config_.GetRootDir().ToUTF8() + "\"" ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddExerciseArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddExerciseArgument( const tools::Path& exercise )
{
    AddArgument( QString( "--exercise=\"" ) + exercise.ToUTF8().c_str() +"\"" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddSessionArgument
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddSessionArgument( const tools::Path& session )
{
    AddArgument( QString( "--session=\"" ) + session.ToUTF8().c_str() +"\"" );
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
    LPSTARTUPINFOW wStartupInfo = (LPSTARTUPINFOW) &startupInfo; 

    std::string debug( commandLine_.toStdString() ) ;
    LPWSTR commandLine = (LPWSTR)commandLine_.toStdWString().c_str();
    if( !CreateProcessW( 0,                                     // lpApplicationName
                         commandLine,                           // lpCommandLine
                         0,                                     // lpProcessAttributes
                         0,                                     // lpThreadAttributes
                         TRUE,                                  // bInheritHandles
                         CREATE_NEW_CONSOLE,                    // dwCreationFlags
                         0,                                     // lpEnvironment
                         workingDirectory_.ToUnicode().c_str(), // lpCurrentDirectory
                         wStartupInfo,                          // lpStartupInfo
                         &internal_->pid_) )                    // lpProcessInformation
    {
        DWORD errCode = GetLastError();
        throw MASA_EXCEPTION( tools::translate( "SpawnCommand", "Could not start process: %1, error: %2" ).arg( debug.c_str() ).arg( errCode ).toStdString().c_str() );
    }

    if ( HANDLE jobObject = OpenJobObject( JOB_OBJECT_ALL_ACCESS, TRUE, jobName_.c_str() ) )
        AssignProcessToJobObject( jobObject, internal_->pid_.hProcess );
}

namespace
{
    BOOL CALLBACK CloseWndProc( HWND hwnd, LPARAM /*lParam*/ )
    {
        if( IsWindow( hwnd ) )
        {
            DWORD_PTR result;
            SendMessageTimeout( hwnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 0, &result );
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
void SpawnCommand::StopProcess()
{
    if( internal_.get() && internal_->pid_.hProcess )
    {
        HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
        THREADENTRY32 te32;

        // Take a snapshot of all running threads
        hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, internal_->pid_.dwThreadId );
        if( hThreadSnap == INVALID_HANDLE_VALUE )
            return;
        te32.dwSize = sizeof( THREADENTRY32 );
        if( !Thread32First( hThreadSnap, &te32 ) )
        {
            CloseHandle( hThreadSnap ); // Must clean up the snapshot object!
            return;
        }
        do
        {
            if( te32.th32OwnerProcessID == internal_->pid_.dwProcessId )
                EnumThreadWindows( te32.th32ThreadID, &::CloseWndProc, 0 );
        }
        while( Thread32Next( hThreadSnap, &te32 ) );
        EnumThreadWindows( internal_->pid_.dwThreadId, &::CloseWndProc, 0 );
        TerminateProcess( internal_->pid_.hProcess, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Stop
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
void SpawnCommand::Stop()
{
    stopped_ = true;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetPid
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
int SpawnCommand::GetPid() const
{
    if( !internal_.get() )
        return -1;
    return int( internal_->pid_.dwProcessId );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Wait
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
bool SpawnCommand::Wait( const boost::posix_time::time_duration& duration )
{
    if( !internal_.get() )
        return true;
    const auto rpy = WaitForSingleObjectEx( internal_->pid_.hProcess,
                                            int( duration.total_milliseconds() ),
                                            false );
    return rpy != WAIT_OBJECT_0;
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
    StopProcess();
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
// Name: SpawnCommand::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetStartedExercise() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetExercise
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetExercise() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetSession
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path SpawnCommand::GetSession() const
{
    return tools::Path();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::SetWorkingDirectory
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void SpawnCommand::SetWorkingDirectory( const tools::Path& directory )
{
    workingDirectory_ = directory;
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::GetCommanderEndpoint
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
const std::string& SpawnCommand::GetCommanderEndpoint() const
{
    return networkCommanderEndpoint_;
}
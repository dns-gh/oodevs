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
SpawnCommand::SpawnCommand( const tools::GeneralConfig& config, const char* exe, bool attach )
    : config_( config )
    , internal_( new InternalData() )
    , attach_( attach )
{
// $$$$ AGE 2007-10-09: 
//    connect( this, SIGNAL( processExited() ), parent, SLOT( OnExit() ) );
//    connect( this, SIGNAL( processExited() ), SLOT( deleteLater() ) );
    addArgument( exe );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::~SpawnCommand()
{
    if( attach_ )
        Stop();
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddRootDirArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddRootDirArgument()
{
    addArgument( ( "--root-dir=\"" + config_.GetRootDir() + "\"" ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddExerciseArgument
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddExerciseArgument( const QString& exercise )
{
    addArgument( "--exercise=\"" + exercise +"\"" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::AddSessionArgument
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
void SpawnCommand::AddSessionArgument( const QString& session )
{
    addArgument( "--session=\"" + session +"\"" );
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
    if( !CreateProcessA( 0, commandLine_.local8Bit().data(),
                0, 0, TRUE, CREATE_NEW_CONSOLE, 0,
                ".", &startupInfo, &internal_->pid_) )
    {
        DWORD errCode = GetLastError();
        throw std::runtime_error( "Could not start process" );
    }
}

namespace
{
    BOOL CALLBACK CloseWndProc( HWND hwnd, LPARAM /*lParam*/ )
    {
        if( IsWindow( hwnd ) )
        {
            DWORD result;
            SendMessageTimeout( hwnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 0, &result );
        }
        return TRUE;
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::CloseWindows
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
void SpawnCommand::CloseWindows()
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
    } while( Thread32Next( hThreadSnap, &te32 ) );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Stop
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
void SpawnCommand::Stop()
{
    if( internal_->pid_.hProcess )
    {
        CloseWindows();
        TerminateProcess( internal_->pid_.hProcess, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Wait
// Created: RDS 2008-08-25
// -----------------------------------------------------------------------------
bool SpawnCommand::Wait()
{
    if( internal_->pid_.hProcess )
        WaitForSingleObject( internal_->pid_.hProcess, INFINITE );
    return true;
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
void SpawnCommand::addArgument( QString arg )
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
std::string SpawnCommand::GetStartedExercise() const
{
    return std::string();
}

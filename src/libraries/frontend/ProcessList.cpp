    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ProcessList.h"
#include <windows.h>
#include <tlhelp32.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace frontend;

class ProcessList::ProcInfo : private boost::noncopyable
{
public:
    ProcInfo( const DWORD pid ) : pid_ ( pid ) {}
    bool Kill() const;
    bool IsCurrentProcess() const;
private:
    DWORD pid_;
};

// -----------------------------------------------------------------------------
// Name: ProcessList constructor
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
ProcessList::ProcessList()
{
    // Take a snapshot of all processes in the system.
    HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
        throw MASA_EXCEPTION( "Unable to retrieve process list" );

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        CloseHandle( hProcessSnap );
        throw MASA_EXCEPTION( "Unable to retrieve process list" );
    }

    DWORD currentProcessID = GetCurrentProcessId();
    do
    {
        if( pe32.th32ProcessID != currentProcessID )
            processes_.insert( pe32.szExeFile, new ProcInfo( pe32.th32ProcessID ) );
    }
    while( Process32Next( hProcessSnap, &pe32 ) );
    CloseHandle( hProcessSnap );
}

// -----------------------------------------------------------------------------
// Name: ProcessList destructor
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
ProcessList::~ProcessList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessList::Contains
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
bool ProcessList::Contains( const std::string& procName ) const
{
    return processes_.count( procName ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ProcessList::KillAll
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
unsigned int ProcessList::KillAll( const std::string& procName )
{
    unsigned int killedProcesses = 0;
    for( T_ProcList::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
        if( it->first == procName
         && !it->second->IsCurrentProcess()
         &&  it->second->Kill() )
            ++killedProcesses;
    return killedProcesses;
}

// -----------------------------------------------------------------------------
// Name: ProcessList::ProcessInfo::Kill
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
bool ProcessList::ProcInfo::Kill() const
{
    // Retrieve the process handle
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid_ );
    // Terminates the process
    if( hProcess == NULL )
        return false;
    else
    {
        TerminateProcess( hProcess, 1 );
        CloseHandle( hProcess );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessList::ProcessInfo::Kill
// Created: RDS 2008-08-20
// -----------------------------------------------------------------------------
bool ProcessList::ProcInfo::IsCurrentProcess() const
{
    // Retrieve the process handle
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid_ );
    return hProcess == GetCurrentProcess();
}

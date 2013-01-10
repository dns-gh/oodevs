// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartDispatcher.h"
#include "clients_kernel/Tools.h"
#include "tools/IpcQueue.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartDispatcher constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
StartDispatcher::StartDispatcher( const tools::GeneralConfig& config,
                                  bool attach,
                                  const QString& exercise,
                                  const QString& session,
                                  const QString& checkpoint /*= ""*/,
                                  const QString& dispatcher_path /*= ""*/,
                                  std::string commanderEndpoint /*= ""*/,
                                  std::string processJobName /*= ""*/ )
    : SpawnCommand( config, "dispatcher_app.exe", attach, commanderEndpoint, processJobName )
{
    if( !dispatcher_path.isEmpty() )
        SetWorkingDirectory( dispatcher_path );
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    if( !checkpoint.isEmpty() )
        AddArgument( "--checkpoint=" + checkpoint );
}

// -----------------------------------------------------------------------------
// Name: StartDispatcher::~StartDispatcher
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
StartDispatcher::~StartDispatcher()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: StartDispatcher::Stop
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
void StartDispatcher::Stop()
{
    if( tools::ipc::Queue::Send( tools::ipc::IPC_COMMAND_STOP, GetPid() ) )
        Wait( boost::posix_time::seconds( 10 ) );
    SpawnCommand::Stop();
}
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
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartDispatcher constructor
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
StartDispatcher::StartDispatcher( const tools::GeneralConfig& config,
                                  const tools::Path& exercise,
                                  const tools::Path& session,
                                  const tools::Path& checkpoint,
                                  const tools::Path& dispatcher_path,
                                  const std::string& name )
    : SpawnCommand( config, MakeBinaryName( "dispatcher_app" ), name )
{
    if( !dispatcher_path.IsEmpty() )
        SetWorkingDirectory( dispatcher_path );
    AddRootArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    if( !checkpoint.IsEmpty() )
        AddArgument( "checkpoint", checkpoint.ToUTF8() );
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
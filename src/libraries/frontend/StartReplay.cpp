// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartReplay.h"
#include "ConfigurationManipulator.h"
#include "SimulationMonitor.h"
#include "tools/IpcQueue.h"
#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartReplay constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::StartReplay( const tools::GeneralConfig& config,
                          const tools::Path& exercise,
                          const tools::Path& session,
                          const std::string& name )
    : SpawnCommand( config, MakeBinaryName( "replayer_app" ), name )
    , configManipulator_( new ConfigurationManipulator( config, exercise, session ) )
{
    AddRootArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
}

// -----------------------------------------------------------------------------
// Name: StartReplay destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::~StartReplay()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: StartReplay::Start
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
void StartReplay::Start()
{
    SpawnCommand::Start();
    const std::string host = configManipulator_->GetValue< std::string >( "session/config/gaming/network/@server" );
    SimulationMonitor monitor( host );
    while( !monitor.Connected() )
    {
        monitor.Update();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    }
    boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
}

// -----------------------------------------------------------------------------
// Name: StartReplay::Stop
// Created: BAX 2013-01-09
// -----------------------------------------------------------------------------
void StartReplay::Stop()
{
    if( tools::ipc::Queue::Send( tools::ipc::IPC_COMMAND_STOP, GetPid() ) )
        Wait( boost::posix_time::seconds( 10 ) );
    SpawnCommand::Stop();
}

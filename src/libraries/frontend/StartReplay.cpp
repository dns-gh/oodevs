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
                          const QString& exercise,
                          const QString& session,
                          unsigned port, bool attach,
                          std::string commanderEndpoint /*= ""*/,
                          bool fromLauncher /*= false */  )
                          : SpawnCommand( config, "replayer_app.exe", attach, commanderEndpoint, fromLauncher )
    , configManipulator_( new ConfigurationManipulator( config, exercise.ascii(), session.ascii() ) )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument ( session );
    AddArgument( QString( "--port=%1" ).arg( port ) );
}

// -----------------------------------------------------------------------------
// Name: StartReplay destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::~StartReplay()
{
    // NOTHING
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
    while( !monitor.Connected() && !monitor.TimedOut() )
    {
        monitor.Update();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    }
    boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
}

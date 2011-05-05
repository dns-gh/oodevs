// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartEdxl.h"
#include "ConfigurationManipulator.h"
#include "SimulationMonitor.h"
#include "tools/GeneralConfig.h"

#include <boost/filesystem.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartEdxl constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartEdxl::StartEdxl( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach )
    : SpawnCommand( config, "edxlhave_app.exe", attach )
    , configManipulator_( new ConfigurationManipulator( config, exercise.ascii(), session.ascii() ) )
{
    namespace bfs = boost::filesystem;
    bfs::path iniFile = bfs::path( config.GetExerciseDir( exercise.ascii() ) ) / "edxl.ini";
    mustRun_ = bfs::exists( iniFile );

    if( mustRun_ )
    {
        AddArgument( ( "--sword.root=\"" + config_.GetRootDir() + "\"" ).c_str() );
        AddArgument( "--sword.exercise=\"" + exercise + "\"" );
        AddArgument( QString( "--ini=\"" ) + iniFile.string().c_str() + "\"" );
    }
}

// -----------------------------------------------------------------------------
// Name: StartEdxl destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartEdxl::~StartEdxl()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartEdxl::Start
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
void StartEdxl::Start()
{
    if( mustRun_ )
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

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "SpawnCommand.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: SpawnCommand constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::SpawnCommand( QObject* parent, const tools::GeneralConfig& config, const char* exe )
    : QProcess( parent )
    , config_( config )
{
    connect( this, SIGNAL( processExited() ), parent, SLOT( OnExit() ) );
    connect( this, SIGNAL( processExited() ), SLOT( deleteLater() ) );
    addArgument( exe );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
SpawnCommand::~SpawnCommand()
{
    // NOTHING
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
    addArgument( "--exercise=\"" + exercise + "\"" );
}

// -----------------------------------------------------------------------------
// Name: SpawnCommand::Start
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void SpawnCommand::Start()
{
    if( ! start() )
        throw std::runtime_error( "Could not start process" );
}

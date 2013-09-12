// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "JoinExercise.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: JoinExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercise::JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
    : SpawnCommand( config, MakeBinaryName( "gaming_app" ), "gaming" )
{
    AddRootArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
}

// -----------------------------------------------------------------------------
// Name: JoinExercise constructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
JoinExercise::JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const QString& profile )
    : SpawnCommand( config, MakeBinaryName( "gaming_app" ), "gaming" )
{
    AddRootArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    if( profile.isEmpty() )
        AddArgument( "login", "anonymous" );
    else
        AddArgument( "login", profile.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: JoinExercise destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercise::~JoinExercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: JoinExercise::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString JoinExercise::GetStatus() const
{
    if( GetPercentage() < 100 )
        return tools::translate( "JoinExercise", "Starting user interface..." );
    return tools::translate( "JoinExercise", "User interface started" );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "JoinAnalysis.h"
#include "GameConfigPanel.h"
#pragma warning( disable: 4127 4511 4512 )
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: JoinAnalysis constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::JoinAnalysis( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, unsigned number )
    : SpawnCommand( parent, config, "gaming_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    const std::string host = "--host=localhost:"  // $$$$ AGE 2008-01-07: 
                           + boost::lexical_cast< std::string >( 
                                GameConfigPanel::DispatcherPort( number ) );
    addArgument( host.c_str() );
    Start();
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysis destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::~JoinAnalysis()
{
    // NOTHING
}
